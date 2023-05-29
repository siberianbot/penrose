#include "RenderGraphExecutor.hpp"

#include <utility>

#include "src/Events/EventQueue.hpp"
#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/DeviceMemoryAllocator.hpp"
#include "src/Rendering/PresentContext.hpp"
#include "src/Rendering/RenderContext.hpp"
#include "src/Rendering/RenderGraphParser.hpp"
#include "src/Resources/ResourceSet.hpp"
#include "src/Utils/OptionalUtils.hpp"

namespace Penrose {

    RenderGraphExecutor::SwapchainTarget::SwapchainTarget(PresentContext *presentContext)
            : _presentContext(presentContext) {
        //
    }

    const vk::ImageView &RenderGraphExecutor::SwapchainTarget::getView(const uint32_t &imageIdx) const {
        return this->_presentContext->getSwapchainImageViews().at(imageIdx);
    }

    const vk::Extent2D &RenderGraphExecutor::SwapchainTarget::getExtent() const {
        return this->_presentContext->getSwapchainExtent();
    }

    RenderGraphExecutor::ImageTarget::ImageTarget(DeviceContext *deviceContext,
                                                  DeviceMemoryAllocator *deviceMemoryAllocator,
                                                  vk::Image image,
                                                  vk::ImageView imageView,
                                                  vk::Extent2D extent)
            : _deviceContext(deviceContext),
              _deviceMemoryAllocator(deviceMemoryAllocator),
              _image(image),
              _imageView(imageView),
              _extent(extent) {
        //
    }

    RenderGraphExecutor::ImageTarget::~ImageTarget() {
        this->_deviceContext->getLogicalDevice().destroy(this->_imageView);

        this->_deviceMemoryAllocator->allocateDeviceLocalFor(this->_image);
        this->_deviceContext->getLogicalDevice().destroy(this->_image);
    }

    RenderGraphExecutor::Pass::Pass(DeviceContext *deviceContext,
                                    vk::RenderPass renderPass,
                                    std::vector<vk::ClearValue> clearValues,
                                    std::vector<std::uint32_t> targets,
                                    std::vector<std::optional<std::unique_ptr<RenderOperator>>> operators)
            : _deviceContext(deviceContext),
              _renderPass(renderPass),
              _clearValues(std::move(clearValues)),
              _targets(std::move(targets)),
              _operators(std::move(operators)) {
        //
    }

    RenderGraphExecutor::Pass::~Pass() {
        this->_deviceContext->getLogicalDevice().destroy(this->_renderPass);
    }

    RenderGraphExecutor::Framebuffer::Framebuffer(DeviceContext *deviceContext,
                                                  std::vector<vk::Framebuffer> framebuffers)
            : _deviceContext(deviceContext),
              _framebuffers(std::move(framebuffers)) {
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
            auto image = this->_deviceContext->getLogicalDevice().createImage(imageCreateInfo);

            this->_deviceMemoryAllocator->allocateDeviceLocalFor(image);

            auto imageViewCreateInfo = parser.parse<RenderTarget, vk::ImageViewCreateInfo>(target)
                    .setImage(image);
            auto imageView = this->_deviceContext->getLogicalDevice().createImageView(imageViewCreateInfo);

            auto extent = vk::Extent2D(imageCreateInfo.extent.width, imageCreateInfo.extent.height);

            return std::make_unique<RenderGraphExecutor::ImageTarget>(this->_deviceContext,
                                                                      this->_deviceMemoryAllocator,
                                                                      image,
                                                                      imageView,
                                                                      extent);
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

            auto producer = this->_renderContext->tryGetRenderOperatorProducer(*subgraph.passes.at(idx).operatorName);

            if (!producer.has_value()) {
                // TODO: notify
                operators[idx] = std::nullopt;
                continue;
            }

            auto context = RenderOperatorProduceContext{
                    .resources = this->_resources,
                    .renderPass = renderPass,
                    .subpassIdx = idx
            };

            operators[idx] = (*producer)->produce(context);
        }

        return std::make_unique<RenderGraphExecutor::Pass>(this->_deviceContext,
                                                           renderPass,
                                                           clearValues,
                                                           targets,
                                                           std::move(operators));
    }

    std::unique_ptr<RenderGraphExecutor::Framebuffer> RenderGraphExecutor::createFramebuffer(
            const GraphState &graphState,
            const std::unique_ptr<Pass> &pass) {
        auto imageCount = this->_presentContext->getSwapchainImageViews().size();
        auto framebuffers = std::vector<vk::Framebuffer>(imageCount);

        for (std::uint32_t imageIdx = 0; imageIdx < imageCount; imageIdx++) {
            auto attachments = std::vector<vk::ImageView>(pass->getTargets().size());
            std::optional<vk::Extent2D> extent;

            for (std::uint32_t attachmentIdx = 0; attachmentIdx < pass->getTargets().size(); attachmentIdx++) {
                auto &target = graphState.targets.at(pass->getTargets().at(attachmentIdx));

                if (extent.has_value() && target->getExtent() != *extent) {
                    throw EngineError("Pass contains attachment of different size");
                } else if (!extent.has_value()) {
                    extent = target->getExtent();
                }

                attachments[attachmentIdx] = target->getView(imageIdx);
            }

            auto createInfo = vk::FramebufferCreateInfo()
                    .setRenderPass(pass->getRenderPass());

            if (!extent.has_value()) {
                createInfo.setFlags(vk::FramebufferCreateFlagBits::eImageless);
            } else {
                createInfo
                        .setWidth(extent->width)
                        .setHeight(extent->height)
                        .setAttachments(attachments)
                        .setLayers(1);
            }

            framebuffers[imageIdx] = this->_deviceContext->getLogicalDevice().createFramebuffer(createInfo);
        }

        return std::make_unique<RenderGraphExecutor::Framebuffer>(this->_deviceContext, framebuffers);
    }

    RenderGraphExecutor::GraphState RenderGraphExecutor::createGraphState(const RenderGraph &graph) {
        auto state = RenderGraphExecutor::GraphState{
                .graph = graph,
                .targets = std::vector<std::unique_ptr<Target>>(graph.targets.size()),
                .passes = std::vector<std::unique_ptr<Pass>>(graph.subgraphs.size())
        };

        for (std::uint32_t idx = 0; idx < graph.targets.size(); idx++) {
            state.targets[idx] = this->createTarget(graph.targets.at(idx));
        }

        for (std::uint32_t idx = 0; idx < graph.subgraphs.size(); idx++) {
            state.passes[idx] = this->createPass(graph.subgraphs.at(idx));
        }

        return state;
    }

    RenderGraphExecutor::FramebufferState RenderGraphExecutor::createFramebufferState(
            const RenderGraphExecutor::GraphState &graphState) {
        auto state = RenderGraphExecutor::FramebufferState{
                .framebuffers = std::vector<std::unique_ptr<Framebuffer>>(graphState.passes.size())
        };

        for (std::uint32_t idx = 0; idx < graphState.passes.size(); idx++) {
            state.framebuffers[idx] = this->createFramebuffer(graphState, graphState.passes.at(idx));
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
              _deviceMemoryAllocator(resources->get<DeviceMemoryAllocator>()),
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
            if (event.type != EventType::RenderContextModified) {
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
            auto framebuffer = currentFramebufferState.framebuffers.at(passIdx)->getFramebuffer(imageIdx);

            auto renderPassBeginInfo = vk::RenderPassBeginInfo()
                    .setRenderPass(pass->getRenderPass())
                    .setFramebuffer(framebuffer)
                    .setClearValues(pass->getClearValues());

            commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

            for (auto it = pass->getOperators().begin(); it != pass->getOperators().end(); it++) {
                if (it != pass->getOperators().begin()) {
                    commandBuffer.nextSubpass(vk::SubpassContents::eInline);
                }

                if (!it->has_value()) {
                    continue;
                }

                (**it)->execute(commandBuffer);
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

