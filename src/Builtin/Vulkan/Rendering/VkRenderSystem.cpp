#include "VkRenderSystem.hpp"

#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    static constexpr const std::uint64_t MAX_TIMEOUT = std::numeric_limits<std::uint64_t>::max();

    VkRenderSystem::VkRenderSystem(ResourceSet *resources)
            : _commandManager(resources->getLazy<VkCommandManager>()),
              _eventQueue(resources->getLazy<EventQueue>()),
              _logicalDeviceContext(resources->getLazy<VkLogicalDeviceContext>()),
              _renderGraphContext(resources->getLazy<RenderGraphContext>()),
              _renderGraphExecutorProvider(resources->getLazy<RenderGraphExecutorProvider>()),
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

        this->_eventHandlerIdx = this->_eventQueue->addHandler([this](const Event &event) {
            switch (event.type) {
                case EventType::SurfaceResized:
                    this->_swapchainModified = true;
                    break;

                default:
                    break;
            }
        });
    }

    void VkRenderSystem::destroy() {
        this->_eventQueue->removeHandler(this->_eventHandlerIdx);

        this->_currentRenderGraphExecutor = std::nullopt;

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
        if (this->_swapchainInvalid || this->_swapchainModified || this->_renderGraphModified) {
            this->_logicalDeviceContext->getHandle().waitIdle();
        }

        if (this->_renderGraphModified) {
            auto newRenderGraph = this->_renderGraphContext->getRenderGraph();

            if (newRenderGraph.has_value()) {
                auto newExecutor = this->_renderGraphExecutorProvider->createFor(*newRenderGraph);
                newExecutor->createFramebuffers();

                this->_currentRenderGraphExecutor = std::unique_ptr<RenderGraphExecutor>(newExecutor);
            } else {
                this->_currentRenderGraphExecutor = std::nullopt;
            }

            this->_renderGraphModified = false;
        }

        if (this->_swapchainInvalid || this->_swapchainModified) {
            if (this->_currentRenderGraphExecutor.has_value()) {
                (*this->_currentRenderGraphExecutor)->destroyFramebuffers();
            }

            this->_swapchainManager->recreate();

            if (this->_currentRenderGraphExecutor.has_value()) {
                (*this->_currentRenderGraphExecutor)->createFramebuffers();
            }

            this->_swapchainInvalid = false;
            this->_swapchainModified = false;
        }

        try {
            this->_swapchainInvalid = this->renderFrame(this->_frameIdx);

            this->_frameIdx = (this->_frameIdx + 1) % INFLIGHT_FRAME_COUNT;
        } catch (const vk::OutOfDateKHRError &error) {
            this->_swapchainInvalid = true;
        }
    }

    void VkRenderSystem::onRenderGraphModified(const std::optional<RenderGraphInfo> &graphInfo) {
        this->_renderGraphModified = true;
    }

    bool VkRenderSystem::renderFrame(const uint32_t &frameIdx) {
        auto logicalDevice = this->_logicalDeviceContext->getHandle();
        auto graphicsQueue = this->_logicalDeviceContext->getGraphicsQueue();
        auto presentQueue = this->_logicalDeviceContext->getPresentQueue();
        auto swapchain = this->_swapchainManager->getSwapchain()->getHandle();

        auto fence = this->_fences.at(frameIdx);
        auto imageReadySemaphore = this->_imageReadySemaphores.at(frameIdx);
        auto renderFinishedSemaphore = this->_renderFinishedSemaphores.at(frameIdx);

        if (!this->_currentRenderGraphExecutor.has_value()) {
            return false;
        }

        auto fenceResult = logicalDevice.waitForFences(fence, true, MAX_TIMEOUT);
        if (fenceResult == vk::Result::eTimeout) {
            vk::detail::throwResultException(fenceResult, "Fence timeout");
        }

        logicalDevice.resetFences(fence);

        auto [acquireResult, imageIdx] = logicalDevice.acquireNextImageKHR(swapchain, MAX_TIMEOUT,
                                                                           imageReadySemaphore);

        if (acquireResult != vk::Result::eSuccess) {
            return true;
        }

        auto &commandBuffer = this->_commandManager->getGraphicsCommandBuffer(frameIdx);

        auto submits = (*this->_currentRenderGraphExecutor)->execute(commandBuffer,
                                                                     imageReadySemaphore,
                                                                     renderFinishedSemaphore,
                                                                     frameIdx,
                                                                     imageIdx);

        if (submits.empty()) {
            return false;
        }

        graphicsQueue.submit(submits, fence);

        auto presentInfo = vk::PresentInfoKHR()
                .setWaitSemaphores(renderFinishedSemaphore)
                .setSwapchains(swapchain)
                .setImageIndices(imageIdx);

        auto presentResult = presentQueue.presentKHR(presentInfo);

        if (presentResult != vk::Result::eSuccess) {
            return true;
        }

        return false;
    }
}
