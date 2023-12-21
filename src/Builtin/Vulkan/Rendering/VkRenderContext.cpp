#include "VkRenderContext.hpp"

#include <list>

#include <Penrose/Common/EngineError.hpp>

#include "src/Utils/RangeUtils.hpp"

#include "src/Builtin/Vulkan/Rendering/VkRendererContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkUtils.hpp"

namespace Penrose {

    inline static constexpr std::string_view TAG = "VkRenderContext";

    inline constexpr std::uint64_t MAX_FENCE_TIMEOUT = std::numeric_limits<std::uint64_t>::max();
    inline constexpr std::uint64_t MAX_ACQUIRE_TIMEOUT = std::numeric_limits<std::uint64_t>::max();

    static constexpr std::array<vk::PipelineStageFlags, 1> WAIT_DST_STAGE_MASK = {
        vk::PipelineStageFlagBits::eColorAttachmentOutput
    };

    VkRenderContext::VkRenderContext(
        ResourceProxy<Log> log, ResourceProxy<VkLogicalDeviceProvider> logicalDeviceProvider,
        ResourceProxy<VkInternalObjectFactory> internalObjectFactory, ResourceProxy<VkImageFactory> imageFactory,
        ResourceProxy<VkPipelineFactory> pipelineFactory, ResourceProxy<VkSwapchainFactory> swapchainFactory,
        VkSwapchain &&swapchain, vk::UniqueCommandPool &&commandPool, vk::UniqueDescriptorPool &&descriptorPool,
        std::array<FrameData, INFLIGHT_FRAME_COUNT> &&frameData
    )
        : _log(std::move(log)),
          _logicalDeviceProvider(std::move(logicalDeviceProvider)),
          _internalObjectFactory(std::move(internalObjectFactory)),
          _imageFactory(std::move(imageFactory)),
          _pipelineFactory(std::move(pipelineFactory)),
          _swapchainFactory(std::move(swapchainFactory)),
          _commandPool(std::forward<decltype(commandPool)>(commandPool)),
          _descriptorPool(std::forward<decltype(descriptorPool)>(descriptorPool)),
          _swapchain(std::forward<decltype(swapchain)>(swapchain)),
          _frameData(std::forward<decltype(frameData)>(frameData)) {
        //
    }

    VkRenderContext::~VkRenderContext() {
        this->_logicalDeviceProvider->getLogicalDevice().handle->waitIdle();
    }

    bool VkRenderContext::beginRender() {
        auto &device = this->_logicalDeviceProvider->getLogicalDevice().handle;
        auto &frameData = this->_frameData.at(this->_currentFrameIdx);

        vk::Result acquireResult;
        std::uint32_t imageIdx;

        try {
            std::tie(acquireResult, imageIdx) = device->acquireNextImageKHR(
                this->_swapchain.handle.get(), MAX_ACQUIRE_TIMEOUT, frameData.imageReady.get()
            );
        } catch (const vk::OutOfDateKHRError &) {
            return false;
        }

        if (acquireResult != vk::Result::eSuccess) {
            return false;
        }

        const auto fenceResult = device->waitForFences(frameData.fence.get(), true, MAX_FENCE_TIMEOUT);

        if (fenceResult == vk::Result::eTimeout) {
            return false;
        }

        device->resetFences(frameData.fence.get());

        frameData.commandBuffer->reset();
        frameData.commandBuffer->begin(vk::CommandBufferBeginInfo());

        this->_currentState = State {
            .imageIdx = imageIdx,
            .commandBuffer = frameData.commandBuffer.get(),
            .fence = frameData.fence.get(),
            .imageReady = frameData.imageReady.get(),
            .renderFinished = frameData.renderFinished.get(),
        };

        return true;
    }

    void VkRenderContext::submitRender() {
        if (!this->_currentState.has_value()) {
            throw EngineError("Render is not started");
        }

        this->_currentState->commandBuffer.end();

        const auto submits = {
            vk::SubmitInfo()
                .setCommandBuffers(this->_currentState->commandBuffer)
                .setWaitDstStageMask(WAIT_DST_STAGE_MASK)
                .setWaitSemaphores(this->_currentState->imageReady)
                .setSignalSemaphores(this->_currentState->renderFinished),
        };

        this->_logicalDeviceProvider->getLogicalDevice().graphicsQueue.submit(submits, this->_currentState->fence);

        const auto presentInfo = vk::PresentInfoKHR()
                                     .setSwapchains(this->_swapchain.handle.get())
                                     .setWaitSemaphores(this->_currentState->renderFinished)
                                     .setImageIndices(this->_currentState->imageIdx);

        const auto presentResult = this->_logicalDeviceProvider->getLogicalDevice().presentQueue.presentKHR(presentInfo
        );

        if (presentResult != vk::Result::eSuccess) {
            this->invalidate();
        }

        this->_currentFrameIdx = (this->_currentFrameIdx + 1) % INFLIGHT_FRAME_COUNT;

        this->_currentState = std::nullopt;
    }

    RendererContext *VkRenderContext::makeRendererContext() {
        return new VkRendererContext(this, this->_log);
    }

    void VkRenderContext::invalidate() {
        this->_logicalDeviceProvider->getLogicalDevice().handle->waitIdle();

        for (auto &passInfo: this->_passes | std::views::values) {
            passInfo.framebuffers.clear();
        }

        this->_imageTargets.clear();
        this->_swapchain = this->_swapchainFactory->makeSwapchain(this->_swapchain);
    }

    vk::ImageView VkRenderContext::useTarget(const TargetInfo &target) {
        if (target.type == TargetType::Swapchain) {
            return this->_swapchain.imageViews.at(this->_currentState->imageIdx).get();
        }

        if (const auto it = this->_imageTargets.find(target.name); it != this->_imageTargets.end()) {
            const auto &[storedTarget, imageInternal] = it->second;

            if (storedTarget != target) {
                this->_log->writeWarning(
                    TAG, "Attempt to use target {} with different target info, old target info is preserved",
                    target.name
                );
            }

            const auto &[image, imageMemory, imageView] = imageInternal;

            return imageView.get();
        }

        auto [image, imageMemory, imageView] = this->_imageFactory->makeImage(target, this->_swapchain);
        const auto result = imageView.get();

        this->_imageTargets.emplace(
            target.name,
            ImageTarget {
                target,
                VkImageInternal {std::move(image), std::move(imageMemory), std::move(imageView)},
        }
        );

        return result;
    }

    vk::RenderPass VkRenderContext::usePass(const GraphInfo &graph) {
        auto it = this->_passes.find(graph.name);

        if (it == this->_passes.end()) {
            std::tie(it, std::ignore) = this->_passes.emplace(
                graph.name,
                Pass {
                    .info = graph,
                    .pass = this->_internalObjectFactory->makeRenderPass(graph, this->_swapchain),
                }
            );
        }

        return it->second.pass.get();
    }

    vk::Framebuffer VkRenderContext::useFramebuffer(
        const std::initializer_list<TargetInfo> &targets, const GraphInfo &graph, const vk::Extent2D &extent
    ) {
        auto &passInfo = this->_passes.at(graph.name);
        auto targetViews = std::vector<vk::ImageView>(targets.size());

        std::ranges::transform(targets, targetViews.begin(), [&](const auto &target) {
            return this->useTarget(target);
        });

        auto it = passInfo.framebuffers.find(targetViews);

        if (it == passInfo.framebuffers.end()) {
            std::tie(it, std::ignore) = passInfo.framebuffers.emplace(
                targetViews, this->_internalObjectFactory->makeFramebuffer(
                                 passInfo.pass.get(), targetViews, extent.width, extent.height
                             )
            );
        }

        return it->second.get();
    }

    vk::Pipeline VkRenderContext::usePipeline(
        const VkPipeline *pipeline, const vk::RenderPass pass, const std::uint32_t subpass
    ) {
        const auto key = PipelineKey {pipeline->getPipelineInfo().name, pass, subpass};

        auto it = this->_pipelines.find(key);

        if (it == this->_pipelines.end()) {
            const auto pipelineInstance = this->_pipelineFactory->makePipelineInstance(pipeline, pass, subpass);

            auto pipelineData = PipelineData {
                .instance = std::unique_ptr<VkPipelineInstance>(pipelineInstance),
                .descriptors = {},
            };

            std::tie(it, std::ignore) = this->_pipelines.emplace(key, std::move(pipelineData));
        }

        return it->second.instance->getHandle();
    }

    vk::DescriptorSet VkRenderContext::useDescriptor(
        const VkPipeline *pipeline, vk::RenderPass pass, std::uint32_t subpass, const PipelineBindingInfo &binding
    ) {
        const auto &pipelineInfo = pipeline->getPipelineInfo();

        if (pipelineInfo.objects.size() != binding.objects.size()) {
            throw EngineError(
                "Expected {} object(s), got {} object(s)", pipelineInfo.objects.size(), binding.objects.size()
            );
        }

        auto &device = this->_logicalDeviceProvider->getLogicalDevice().handle;

        const auto key = PipelineKey {pipeline->getPipelineInfo().name, pass, subpass};
        auto &pipelineData = this->_pipelines.at(key);

        vk::DescriptorSet descriptor;
        bool update = true;

        if (!binding.tag.has_value()) {
            const auto allocateInfo = vk::DescriptorSetAllocateInfo()
                                          .setDescriptorPool(this->_descriptorPool.get())
                                          .setSetLayouts(pipeline->getDescriptorSetLayoutHandle())
                                          .setDescriptorSetCount(1);

            auto newDescriptors = device->allocateDescriptorSetsUnique(allocateInfo);

            descriptor = newDescriptors.at(0).get();

            this->_currentState->descriptors.insert(std::move(newDescriptors.at(0)));
        } else {
            auto it = pipelineData.descriptors.find(*binding.tag);

            if (it == pipelineData.descriptors.end()) {
                const auto allocateInfo = vk::DescriptorSetAllocateInfo()
                                              .setDescriptorPool(this->_descriptorPool.get())
                                              .setSetLayouts(pipeline->getDescriptorSetLayoutHandle())
                                              .setDescriptorSetCount(INFLIGHT_FRAME_COUNT);

                auto newDescriptors = device->allocateDescriptorSetsUnique(allocateInfo);
                std::array<PipelineDescriptorData, INFLIGHT_FRAME_COUNT> descriptorData;

                for (std::uint32_t descriptorIdx = 0; descriptorIdx < INFLIGHT_FRAME_COUNT; descriptorIdx++) {
                    descriptorData[descriptorIdx].info = {};
                    descriptorData[descriptorIdx].descriptor = std::move(newDescriptors.at(descriptorIdx));
                }

                std::tie(it, std::ignore) = pipelineData.descriptors.emplace(*binding.tag, std::move(descriptorData));
            }

            descriptor = it->second.at(this->_currentFrameIdx).descriptor.get();
            update = it->second.at(this->_currentFrameIdx).info == binding.objects;
        }

        if (update) {
            std::list<std::vector<vk::DescriptorBufferInfo>> buffers;
            std::list<std::vector<vk::DescriptorImageInfo>> images;
            auto writes = std::vector<vk::WriteDescriptorSet>(binding.objects.size());

            for (std::uint32_t objectIdx = 0; objectIdx < binding.objects.size(); objectIdx++) {
                const auto &objectInfo = pipelineInfo.objects.at(objectIdx);
                const auto &objectBinding = binding.objects.at(objectIdx);

                auto write = vk::WriteDescriptorSet()
                                 .setDstSet(descriptor)
                                 .setDstBinding(objectIdx)
                                 .setDstArrayElement(0)
                                 .setDescriptorCount(objectInfo.count)
                                 .setDescriptorType(mapObjectType(objectInfo.type));

                switch (objectInfo.type) {
                    case PipelineObjectType::InputAttachment:
                        // nothing to do
                        break;

                    case PipelineObjectType::Sampler:
                        {
                            auto &items = images.emplace_back();
                            const auto count = objectBinding.images.size();

                            if (count != objectBinding.samplers.size()) {
                                throw EngineError(
                                    "Object binding {} have {} image(s) and {} sampler(s)", objectIdx,
                                    objectBinding.images.size(), objectBinding.samplers.size()
                                );
                            }

                            for (std::uint32_t idx = 0; idx < count; idx++) {
                                items.emplace_back(
                                    asVkSampler(objectBinding.samplers.at(idx))->getHandle(),
                                    asVkImage(objectBinding.images.at(idx))->getViewHandle(),
                                    vk::ImageLayout::eShaderReadOnlyOptimal
                                );
                            }

                            write = write.setImageInfo(items);
                            break;
                        }

                    case PipelineObjectType::UniformBuffer:
                        {
                            auto &items = buffers.emplace_back();
                            const auto count = objectBinding.buffers.size();

                            for (std::uint32_t idx = 0; idx < count; idx++) {
                                items.emplace_back(
                                    asVkBuffer(objectBinding.buffers.at(idx))->getHandle(), 0,
                                    objectBinding.buffers.at(idx)->getSize()
                                );
                            }

                            write = write.setBufferInfo(items);
                            break;
                        }

                    default:
                        throw EngineError::notImplemented();
                }

                writes[objectIdx] = write;
            }

            device->updateDescriptorSets(writes, {});
        }

        return descriptor;
    }
}
