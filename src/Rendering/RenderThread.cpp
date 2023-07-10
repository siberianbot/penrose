#include "RenderThread.hpp"

#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/PresentContext.hpp"
#include "src/Rendering/RenderGraphExecutor.hpp"

namespace Penrose {

    static constexpr const std::uint64_t MAX_TIMEOUT = std::numeric_limits<std::uint64_t>::max();

    bool RenderThread::renderFrame(const std::uint32_t &frameIdx) {
        auto logicalDevice = this->_deviceContext->getLogicalDevice();
        auto presentQueue = this->_deviceContext->getPresentQueue();
        auto swapchain = this->_presentContext->getSwapchain();

        auto fence = this->_fences.at(frameIdx);
        auto imageReadySemaphore = this->_renderGraphExecutor->getImageReadySemaphores().at(frameIdx);
        auto graphExecutedSemaphore = this->_renderGraphExecutor->getGraphExecutedSemaphores().at(frameIdx);

        auto fenceResult = logicalDevice.waitForFences(fence, true, MAX_TIMEOUT);
        if (fenceResult == vk::Result::eTimeout) {
            vk::throwResultException(fenceResult, "Fence timeout");
        }

        logicalDevice.resetFences(fence);

        auto [acquireResult, imageIdx] = logicalDevice.acquireNextImageKHR(swapchain, MAX_TIMEOUT,
                                                                           imageReadySemaphore);

        if (acquireResult != vk::Result::eSuccess) {
            return true;
        }

        this->_renderGraphExecutor->execute(frameIdx, imageIdx, fence);

        auto presentInfo = vk::PresentInfoKHR()
                .setWaitSemaphores(graphExecutedSemaphore)
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
              _presentContext(resources->get<PresentContext>()),
              _renderGraphExecutor(resources->get<RenderGraphExecutor>()) {
        //
    }

    void RenderThread::init() {
        auto logicalDevice = this->_deviceContext->getLogicalDevice();

        auto fenceCreateInfo = vk::FenceCreateInfo()
                .setFlags(vk::FenceCreateFlagBits::eSignaled);

        for (std::uint32_t idx = 0; idx < INFLIGHT_FRAME_COUNT; idx++) {
            this->_fences[idx] = logicalDevice.createFence(fenceCreateInfo);
        }

        this->_thread = std::jthread([this](const std::stop_token &stopToken) {
            bool swapchainInvalid = false;

            while (!stopToken.stop_requested()) {
                auto swapchainLock = this->_presentContext->acquireSwapchainLock();
                auto renderGraphExecutorLock = this->_renderGraphExecutor->acquireExecutorLock();

                if (swapchainInvalid) {
                    this->_deviceContext->getLogicalDevice().waitIdle();

                    this->_renderGraphExecutor->invalidate();
                    this->_presentContext->recreate();

                    swapchainInvalid = false;
                }

                try {
                    swapchainInvalid = this->renderFrame(this->_currentFrameIdx);

                    this->_currentFrameIdx = (this->_currentFrameIdx + 1) % INFLIGHT_FRAME_COUNT;
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

        for (const auto &fence: this->_fences) {
            logicalDevice.destroy(fence);
        }
    }
}
