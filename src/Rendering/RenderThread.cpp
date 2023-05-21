#include "RenderThread.hpp"

#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/PresentContext.hpp"
#include "src/Resources/ResourceSet.hpp"

namespace Penrose {

    static constexpr const std::uint32_t INFLIGHT_FRAMES = 2;
    static constexpr const std::uint64_t MAX_TIMEOUT = std::numeric_limits<std::uint64_t>::max();

    bool RenderThread::renderFrame(const std::uint32_t &frameIdx) {
        auto logicalDevice = this->_deviceContext->getLogicalDevice();
        auto graphicsQueue = this->_deviceContext->getGraphicsQueue();
        auto presentQueue = this->_deviceContext->getPresentQueue();
        auto swapchain = this->_presentContext->getSwapchain();

        auto frameSync = this->_syncs[frameIdx];

        auto fenceResult = logicalDevice.waitForFences(frameSync.fence, true, MAX_TIMEOUT);
        if (fenceResult == vk::Result::eTimeout) {
            vk::throwResultException(fenceResult, "Fence timeout");
        }

        logicalDevice.resetFences(frameSync.fence);

        auto [acquireResult, imageIdx] = logicalDevice.acquireNextImageKHR(swapchain, MAX_TIMEOUT,
                                                                           frameSync.imageAvailable);

        if (acquireResult != vk::Result::eSuccess) {
            return true;
        }

        // TODO: remove stub and draw frame
        auto memoryBarrier = vk::ImageMemoryBarrier()
                .setImage(this->_presentContext->getSwapchainImages().at(imageIdx))
                .setOldLayout(vk::ImageLayout::eUndefined)
                .setNewLayout(vk::ImageLayout::ePresentSrcKHR)
                .setSrcAccessMask(vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite)
                .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite)
                .setSubresourceRange(vk::ImageSubresourceRange()
                                             .setAspectMask(vk::ImageAspectFlagBits::eColor)
                                             .setBaseArrayLayer(0)
                                             .setLayerCount(1)
                                             .setBaseMipLevel(0)
                                             .setLevelCount(1));

        auto commandBuffer = this->_commandBuffers.at(frameIdx);
        commandBuffer.reset();
        commandBuffer.begin(vk::CommandBufferBeginInfo());
        commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                      vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                      vk::DependencyFlagBits::eByRegion,
                                      {}, {}, memoryBarrier);
        commandBuffer.end();

        std::vector<vk::PipelineStageFlags> waitDstStageMask = {
                vk::PipelineStageFlagBits::eColorAttachmentOutput
        };

        auto submitInfo = vk::SubmitInfo()
                .setCommandBuffers(commandBuffer)
                .setWaitDstStageMask(waitDstStageMask)
                .setWaitSemaphores(frameSync.imageAvailable)
                .setSignalSemaphores(frameSync.renderFinished);

        graphicsQueue.submit(submitInfo, frameSync.fence);

        auto presentInfo = vk::PresentInfoKHR()
                .setWaitSemaphores(frameSync.renderFinished)
                .setSwapchains(swapchain)
                .setImageIndices(imageIdx);

        auto presentResult = presentQueue.presentKHR(presentInfo);

        if (presentResult != vk::Result::eSuccess) {
            return true;
        }

        return false;
    }

    RenderThread::RenderThread(ResourceSet *resources)
            : _deviceContext(resources->get<DeviceContext>()),
              _presentContext(resources->get<PresentContext>()) {
        //
    }

    void RenderThread::init() {
        auto logicalDevice = this->_deviceContext->getLogicalDevice();

        // TODO: remove command buffer stub
        auto commandPoolCreateInfo = vk::CommandPoolCreateInfo()
                .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
                .setQueueFamilyIndex(this->_deviceContext->getGraphicsQueueFamily());
        this->_commandPool = logicalDevice.createCommandPool(commandPoolCreateInfo);

        auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo()
                .setCommandPool(this->_commandPool)
                .setCommandBufferCount(INFLIGHT_FRAMES);
        this->_commandBuffers = logicalDevice.allocateCommandBuffers(commandBufferAllocateInfo);

        auto fenceCreateInfo = vk::FenceCreateInfo()
                .setFlags(vk::FenceCreateFlagBits::eSignaled);
        auto semaphoreCreateInfo = vk::SemaphoreCreateInfo();

        this->_syncs.resize(INFLIGHT_FRAMES);
        for (std::uint32_t idx = 0; idx < INFLIGHT_FRAMES; idx++) {
            this->_syncs[idx] = {
                    .fence = logicalDevice.createFence(fenceCreateInfo),
                    .imageAvailable = logicalDevice.createSemaphore(semaphoreCreateInfo),
                    .renderFinished = logicalDevice.createSemaphore(semaphoreCreateInfo)
            };
        }

        this->_thread = std::jthread([this](const std::stop_token &stopToken) {
            bool swapchainInvalid = false;

            while (!stopToken.stop_requested()) {
                auto lock = this->_presentContext->acquireSwapchainLock();

                if (swapchainInvalid) {
                    this->_presentContext->recreate();
                    swapchainInvalid = false;
                }

                try {
                    this->renderFrame(this->_currentFrameIdx);

                    this->_currentFrameIdx = (this->_currentFrameIdx + 1) % INFLIGHT_FRAMES;
                } catch (const vk::OutOfDateKHRError &error) {
                    swapchainInvalid = true;
                }
            }
        });
    }

    void RenderThread::destroy() {
        if (this->_thread.has_value()) {
            auto &thread = this->_thread.value();

            thread.request_stop();

            if (thread.joinable()) {
                thread.join();
            }

            this->_thread = std::nullopt;
        }

        auto logicalDevice = this->_deviceContext->getLogicalDevice();

        logicalDevice.waitIdle();

        for (const auto &sync: this->_syncs) {
            logicalDevice.destroy(sync.fence);
            logicalDevice.destroy(sync.renderFinished);
            logicalDevice.destroy(sync.imageAvailable);
        }

        logicalDevice.freeCommandBuffers(this->_commandPool, this->_commandBuffers);
        logicalDevice.destroy(this->_commandPool);
    }
}
