#include "VkRenderSystem.hpp"

#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    constexpr static const std::uint64_t MAX_TIMEOUT = std::numeric_limits<std::uint64_t>::max();

    constexpr static const std::array<vk::PipelineStageFlags, 1> WAIT_DST_STAGE_MASK = {
            vk::PipelineStageFlagBits::eColorAttachmentOutput
    };

    VkRenderSystem::VkRenderSystem(ResourceSet *resources)
            : _commandManager(resources->getLazy<VkCommandManager>()),
              _logicalDeviceContext(resources->getLazy<VkLogicalDeviceContext>()),
              _renderGraphContextManager(resources->getLazy<VkRenderGraphContextManager>()),
              _renderGraphExecutor(resources->getLazy<VkRenderGraphExecutor>()),
              _swapchainManager(resources->getLazy<VkSwapchainManager>()) {
        //
    }

    void VkRenderSystem::init() {
        auto fenceCreateInfo = vk::FenceCreateInfo().setFlags(vk::FenceCreateFlagBits::eSignaled);
        auto semaphoreCreateInfo = vk::SemaphoreCreateInfo();

        for (std::uint32_t idx = 0; idx < INFLIGHT_FRAME_COUNT; idx++) {
            this->_fences[idx] = this->_logicalDeviceContext->getHandle().createFence(fenceCreateInfo);
            this->_imageReadySemaphores[idx] = this->_logicalDeviceContext->getHandle()
                    .createSemaphore(semaphoreCreateInfo);
            this->_renderFinishedSemaphores[idx] = this->_logicalDeviceContext->getHandle()
                    .createSemaphore(semaphoreCreateInfo);
        }
    }

    void VkRenderSystem::destroy() {
        for (std::uint32_t frameIdx = 0; frameIdx < INFLIGHT_FRAME_COUNT; frameIdx++) {
            this->_logicalDeviceContext->getHandle().destroy(this->_fences.at(frameIdx));
            this->_logicalDeviceContext->getHandle().destroy(this->_imageReadySemaphores.at(frameIdx));
            this->_logicalDeviceContext->getHandle().destroy(this->_renderFinishedSemaphores.at(frameIdx));
        }
    }

    void VkRenderSystem::stop() {
        this->_logicalDeviceContext->getHandle().waitIdle();
    }

    void VkRenderSystem::renderFrame() {
        bool shouldInvalidate;

        try {
            shouldInvalidate = this->renderFrame(this->_frameIdx);
        } catch (const vk::OutOfDateKHRError &error) {
            shouldInvalidate = true;
        }

        if (shouldInvalidate) {
            this->_logicalDeviceContext->getHandle().waitIdle();

            this->_swapchainManager->recreate();
            this->_renderGraphContextManager->invalidate();
        }

        this->_frameIdx = (this->_frameIdx + 1) % INFLIGHT_FRAME_COUNT;
    }

    bool VkRenderSystem::renderFrame(const uint32_t &frameIdx) {
        auto logicalDevice = this->_logicalDeviceContext->getHandle();
        auto graphicsQueue = this->_logicalDeviceContext->getGraphicsQueue();
        auto presentQueue = this->_logicalDeviceContext->getPresentQueue();
        auto swapchain = this->_swapchainManager->getSwapchain();

        auto fence = this->_fences.at(frameIdx);
        auto imageReadySemaphore = this->_imageReadySemaphores.at(frameIdx);
        auto renderFinishedSemaphore = this->_renderFinishedSemaphores.at(frameIdx);

        auto fenceResult = logicalDevice.waitForFences(fence, true, MAX_TIMEOUT);
        if (fenceResult == vk::Result::eTimeout) {
            vk::detail::throwResultException(fenceResult, "Fence timeout");
        }

        auto graphContext = this->_renderGraphContextManager->acquireContext();
        if (!graphContext->hasGraphInfo()) {
            return false;
        }

        logicalDevice.resetFences(fence);

        auto [acquireResult, imageIdx] = logicalDevice.acquireNextImageKHR(swapchain->getHandle(), MAX_TIMEOUT,
                                                                           imageReadySemaphore);

        if (acquireResult != vk::Result::eSuccess) {
            return true;
        }

        auto &commandBuffer = this->_commandManager->getGraphicsCommandBuffer(frameIdx);

        auto submits = this->_renderGraphExecutor->execute(graphContext.get(),
                                                           commandBuffer,
                                                           imageReadySemaphore,
                                                           renderFinishedSemaphore,
                                                           frameIdx,
                                                           imageIdx);

        if (submits.empty()) {
            return false;
        }

        auto vkSubmits = std::vector<vk::SubmitInfo>(submits.size());
        std::transform(submits.begin(), submits.end(), vkSubmits.begin(),
                       [](const VkRenderGraphExecutor::Submit &submit) {
                           return vk::SubmitInfo()
                                   .setCommandBuffers(submit.commandBuffer)
                                   .setWaitDstStageMask(WAIT_DST_STAGE_MASK)
                                   .setWaitSemaphores(submit.waitSemaphores)
                                   .setSignalSemaphores(submit.signalSemaphores);
                       });

        graphicsQueue.submit(vkSubmits, fence);

        auto presentInfo = vk::PresentInfoKHR()
                .setWaitSemaphores(renderFinishedSemaphore)
                .setSwapchains(swapchain->getHandle())
                .setImageIndices(imageIdx);

        auto presentResult = presentQueue.presentKHR(presentInfo);

        if (presentResult != vk::Result::eSuccess) {
            return true;
        }

        return false;
    }
}
