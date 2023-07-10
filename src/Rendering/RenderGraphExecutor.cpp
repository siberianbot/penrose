#include "RenderGraphExecutor.hpp"

#include <utility>

#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Rendering/RenderContext.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/PresentContext.hpp"
#include "src/Rendering/RenderGraphParser.hpp"

namespace Penrose {

    RenderGraphExecutor::SwapchainTarget::SwapchainTarget(PresentContext *presentContext)
            : _presentContext(presentContext) {
        //
    }

    const vk::ImageView &RenderGraphExecutor::SwapchainTarget::getView(const uint32_t &imageIdx) const {
        return this->_presentContext->getSwapchainImageViews().at(imageIdx);
    }

    RenderGraphExecutor::ImageTarget::ImageTarget(Image &&image,
                                                  DeviceMemory &&deviceMemory,
                                                  ImageView &&imageView)
            : _image(std::move(image)),
              _deviceMemory(std::move(deviceMemory)),
              _imageView(std::move(imageView)) {
        //
    }

    RenderGraphExecutor::Pass::Pass(DeviceContext *deviceContext,
                                    vk::RenderPass renderPass,
                                    std::vector<vk::ClearValue> clearValues,
                                    std::vector<std::uint32_t> targets,
                                    std::vector<std::optional<std::unique_ptr<RenderOperator>>> operators,
                                    std::optional<vk::Extent2D> renderArea)
            : _deviceContext(deviceContext),
              _renderPass(renderPass),
              _clearValues(std::move(clearValues)),
              _targets(std::move(targets)),
              _operators(std::move(operators)),
              _renderArea(renderArea) {
        //
    }

    RenderGraphExecutor::Pass::~Pass() {
        this->_deviceContext->getLogicalDevice().destroy(this->_renderPass);
    }

    RenderGraphExecutor::Framebuffer::Framebuffer(DeviceContext *deviceContext,
                                                  std::vector<vk::Framebuffer> framebuffers,
                                                  vk::Extent2D renderArea)
            : _deviceContext(deviceContext),
              _framebuffers(std::move(framebuffers)),
              _renderArea(renderArea) {
        //
    }

    RenderGraphExecutor::Framebuffer::~Framebuffer() {
        for (const auto &framebuffer: this->_framebuffers) {
            this->_deviceContext->getLogicalDevice().destroy(framebuffer);
        }
    }

    std::unique_ptr<RenderGraphExecutor::Target> RenderGraphExecutor::createTarget(const RenderTarget &target) {
        auto createImageTarget = [this](const RenderTarget &target) {
            auto parser = RenderGraphParser(this->_presentContext);

            auto imageCreateInfo = parser.parse<RenderTarget, vk::ImageCreateInfo>(target);

            auto image = makeImage(this->_deviceContext, imageCreateInfo);
            auto deviceMemory = makeDeviceMemory(this->_deviceContext, image);

            auto imageViewCreateInfo = parser.parse<RenderTarget, vk::ImageViewCreateInfo>(target)
                    .setImage(image.getInstance());
            auto imageView = makeImageView(this->_deviceContext, imageViewCreateInfo);

            return std::make_unique<RenderGraphExecutor::ImageTarget>(std::move(image),
                                                                      std::move(deviceMemory),
                                                                      std::move(imageView));
        };

        switch (target.source) {
            case RenderTargetSource::Swapchain:
                return std::make_unique<RenderGraphExecutor::SwapchainTarget>(this->_presentContext);

            case RenderTargetSource::Image:
                return createImageTarget(target);

            default:
                throw EngineError("Unknown source of render target");
        }
    }

    std::unique_ptr<RenderGraphExecutor::Pass> RenderGraphExecutor::createPass(const RenderSubgraph &subgraph) {
        auto parser = RenderGraphParser(this->_presentContext);

        auto renderPassCreateInfo = parser.parse<RenderSubgraph, vk::RenderPassCreateInfo>(subgraph);
        auto renderPass = this->_deviceContext->getLogicalDevice().createRenderPass(renderPassCreateInfo);

        auto clearValues = std::vector<vk::ClearValue>(subgraph.attachments.size());
        auto targets = std::vector<std::uint32_t>(subgraph.attachments.size());
        for (std::uint32_t idx = 0; idx < subgraph.attachments.size(); idx++) {
            auto attachment = subgraph.attachments.at(idx);

            clearValues[idx] = parser.parse<RenderAttachmentClearValue, vk::ClearValue>(attachment.clearValue);
            targets[idx] = attachment.targetIdx;
        }

        auto operators = std::vector<std::optional<std::unique_ptr<RenderOperator>>>(subgraph.passes.size());
        for (std::uint32_t idx = 0; idx < subgraph.passes.size(); idx++) {
            if (!subgraph.passes.at(idx).operatorName.has_value()) {
                operators[idx] = std::nullopt;
                continue;
            }

            auto operatorName = *subgraph.passes.at(idx).operatorName;
            auto operatorParams = ParamsCollection::merge(
                    this->_renderContext->tryGetRenderOperatorDefaults(operatorName)
                            .value_or(ParamsCollection::empty()),
                    subgraph.passes.at(idx).operatorParams.value_or(ParamsCollection::empty()));

            auto context = RenderOperatorCreateContext{
                    .resources = this->_resources,
                    .params = operatorParams,
                    .renderPass = renderPass,
                    .subpassIdx = idx,
            };

            operators[idx] = this->_renderContext->tryCreateRenderOperator(operatorName, context);
        }

        auto renderArea = map(subgraph.renderArea, [](Size size) {
            auto [w, h] = size;

            return vk::Extent2D()
                    .setWidth(w)
                    .setHeight(h);
        });

        return std::make_unique<RenderGraphExecutor::Pass>(this->_deviceContext,
                                                           renderPass,
                                                           clearValues,
                                                           targets,
                                                           std::move(operators),
                                                           renderArea);
    }

    std::unique_ptr<RenderGraphExecutor::Framebuffer> RenderGraphExecutor::createFramebuffer(
            const FramebufferState &framebufferState,
            const std::unique_ptr<Pass> &pass) {
        vk::Extent2D renderArea = pass->getRenderArea().value_or(this->_presentContext->getSwapchainExtent());

        auto imageCount = this->_presentContext->getSwapchainImageViews().size();
        auto framebuffers = std::vector<vk::Framebuffer>(imageCount);

        for (std::uint32_t imageIdx = 0; imageIdx < imageCount; imageIdx++) {
            auto attachments = std::vector<vk::ImageView>(pass->getTargets().size());

            for (std::uint32_t attachmentIdx = 0; attachmentIdx < pass->getTargets().size(); attachmentIdx++) {
                auto &target = framebufferState.targets.at(pass->getTargets().at(attachmentIdx));

                attachments[attachmentIdx] = target->getView(imageIdx);
            }

            auto createInfo = vk::FramebufferCreateInfo()
                    .setRenderPass(pass->getRenderPass())
                    .setWidth(renderArea.width)
                    .setHeight(renderArea.height)
                    .setAttachments(attachments)
                    .setLayers(1);

            framebuffers[imageIdx] = this->_deviceContext->getLogicalDevice().createFramebuffer(createInfo);
        }

        return std::make_unique<RenderGraphExecutor::Framebuffer>(this->_deviceContext, framebuffers, renderArea);
    }

    RenderGraphExecutor::GraphState RenderGraphExecutor::createGraphState(const RenderGraph &graph) {
        auto state = RenderGraphExecutor::GraphState{
                .graph = graph,
                .passes = std::vector<std::unique_ptr<Pass>>(graph.subgraphs.size())
        };

        for (std::uint32_t idx = 0; idx < graph.subgraphs.size(); idx++) {
            state.passes[idx] = this->createPass(graph.subgraphs.at(idx));
        }

        return state;
    }

    RenderGraphExecutor::FramebufferState RenderGraphExecutor::createFramebufferState(
            const RenderGraphExecutor::GraphState &graphState) {
        auto state = RenderGraphExecutor::FramebufferState{
                .targets = std::vector<std::unique_ptr<Target>>(graphState.graph.targets.size()),
                .framebuffers = std::vector<std::unique_ptr<Framebuffer>>(graphState.passes.size())
        };

        for (std::uint32_t idx = 0; idx < graphState.graph.targets.size(); idx++) {
            state.targets[idx] = this->createTarget(graphState.graph.targets.at(idx));
        }

        for (std::uint32_t idx = 0; idx < graphState.passes.size(); idx++) {
            state.framebuffers[idx] = this->createFramebuffer(state, graphState.passes.at(idx));
        }

        return state;
    }

    void RenderGraphExecutor::cleanup(bool framebuffersOnly) {
        this->_deviceContext->getLogicalDevice().waitIdle();

        if (this->_framebufferState.has_value()) {
            this->_framebufferState = std::nullopt;
        }

        if (framebuffersOnly) {
            return;
        }

        if (this->_graphState.has_value()) {
            this->_graphState = std::nullopt;
        }
    }

    RenderGraphExecutor::RenderGraphExecutor(ResourceSet *resources)
            : _resources(resources),
              _eventQueue(resources->get<EventQueue>()),
              _deviceContext(resources->get<DeviceContext>()),
              _presentContext(resources->get<PresentContext>()),
              _renderContext(resources->get<RenderContext>()) {
        //
    }

    void RenderGraphExecutor::init() {
        auto logicalDevice = this->_deviceContext->getLogicalDevice();
        auto commandPool = this->_deviceContext->getCommandPool();

        auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo()
                .setCommandPool(commandPool)
                .setCommandBufferCount(INFLIGHT_FRAME_COUNT);
        auto commandBuffers = logicalDevice.allocateCommandBuffers(commandBufferAllocateInfo);
        std::move(commandBuffers.begin(), commandBuffers.end(), this->_commandBuffers.begin());

        auto semaphoreCreateInfo = vk::SemaphoreCreateInfo();
        for (std::uint32_t idx = 0; idx < INFLIGHT_FRAME_COUNT; idx++) {
            this->_imageReadySemaphores[idx] = logicalDevice.createSemaphore(semaphoreCreateInfo);
            this->_graphExecutedSemaphores[idx] = logicalDevice.createSemaphore(semaphoreCreateInfo);
        }

        this->_eventHandlerIdx = this->_eventQueue->addHandler([this](const Event &event) {
            if (event.type != EventType::RenderGraphModified) {
                return;
            }

            auto lock = this->acquireExecutorLock();
            this->cleanup(false);
        });
    }

    void RenderGraphExecutor::destroy() {
        if (this->_eventHandlerIdx.has_value()) {
            this->_eventQueue->removeHandler(this->_eventHandlerIdx.value());
            this->_eventHandlerIdx = std::nullopt;
        }

        this->cleanup(false);

        auto logicalDevice = this->_deviceContext->getLogicalDevice();

        for (std::uint32_t idx = 0; idx < INFLIGHT_FRAME_COUNT; idx++) {
            logicalDevice.destroy(this->_imageReadySemaphores[idx]);
            logicalDevice.destroy(this->_graphExecutedSemaphores[idx]);
        }
    }

    void RenderGraphExecutor::execute(const std::uint32_t &frameIdx,
                                      const std::uint32_t &imageIdx,
                                      const vk::Fence &fence) {
        if (!this->_graphState.has_value()) {
            if (!this->_renderContext->getRenderGraph().has_value()) {
                return;
            }

            this->_graphState = this->createGraphState(this->_renderContext->getRenderGraph().value());
        }

        auto &currentGraphState = *this->_graphState;

        if (!this->_framebufferState.has_value()) {
            this->_framebufferState = this->createFramebufferState(currentGraphState);
        }

        auto &currentFramebufferState = *this->_framebufferState;

        auto commandBuffer = this->_commandBuffers.at(frameIdx);
        commandBuffer.reset();

        auto commandBufferBeginInfo = vk::CommandBufferBeginInfo();
        commandBuffer.begin(commandBufferBeginInfo);

        for (std::uint32_t passIdx = 0; passIdx < this->_graphState.value().passes.size(); passIdx++) {
            auto &pass = currentGraphState.passes.at(passIdx);
            auto &framebuffer = currentFramebufferState.framebuffers.at(passIdx);

            auto renderArea = vk::Rect2D()
                    .setOffset(vk::Offset2D())
                    .setExtent(framebuffer->getRenderArea());

            auto renderPassBeginInfo = vk::RenderPassBeginInfo()
                    .setRenderPass(pass->getRenderPass())
                    .setFramebuffer(framebuffer->getFramebuffer(imageIdx))
                    .setClearValues(pass->getClearValues())
                    .setRenderArea(renderArea);

            auto executionContext = RenderOperatorExecutionContext{
                    .frameIdx = frameIdx,
                    .renderArea = renderArea,
                    .commandBuffer = commandBuffer
            };

            commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

            for (auto it = pass->getOperators().begin(); it != pass->getOperators().end(); it++) {
                if (it != pass->getOperators().begin()) {
                    commandBuffer.nextSubpass(vk::SubpassContents::eInline);
                }

                if (!it->has_value()) {
                    continue;
                }

                (**it)->execute(executionContext);
            }

            commandBuffer.endRenderPass();
        }

        commandBuffer.end();

        std::vector<vk::PipelineStageFlags> waitDstStageMask = {
                vk::PipelineStageFlagBits::eColorAttachmentOutput
        };

        auto submits = {
                vk::SubmitInfo()
                        .setCommandBuffers(commandBuffer)
                        .setWaitDstStageMask(waitDstStageMask)
                        .setWaitSemaphores(this->_imageReadySemaphores.at(frameIdx))
                        .setSignalSemaphores(this->_graphExecutedSemaphores.at(frameIdx))
        };

        this->_deviceContext->getGraphicsQueue().submit(submits, fence);
    }

    void RenderGraphExecutor::invalidate() {
        this->cleanup(true);
    }
}

