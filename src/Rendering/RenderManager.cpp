#include "RenderManager.hpp"

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Rendering/RenderContext.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/PresentContext.hpp"
#include "src/Rendering/RenderGraphExecutor.hpp"
#include "src/Rendering/RenderGraphExecutorProvider.hpp"

namespace Penrose {

    static constexpr const std::uint64_t MAX_TIMEOUT = std::numeric_limits<std::uint64_t>::max();

    RenderManager::RenderManager(ResourceSet *resources)
            : _eventQueue(resources->get<EventQueue>()),
              _deviceContext(resources->get<DeviceContext>()),
              _presentContext(resources->get<PresentContext>()),
              _renderContext(resources->get<RenderContext>()),
              _renderGraphExecutorProvider(resources->get<RenderGraphExecutorProvider>()) {
        //
    }

    void RenderManager::init() {
        auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo()
                .setCommandPool(this->_deviceContext->getCommandPool())
                .setCommandBufferCount(INFLIGHT_FRAME_COUNT)
                .setLevel(vk::CommandBufferLevel::ePrimary);
        auto commandBuffers = this->_deviceContext->getLogicalDevice()
                .allocateCommandBuffers(commandBufferAllocateInfo);

        std::copy(commandBuffers.begin(), commandBuffers.end(), this->_commandBuffers.begin());

        auto fenceCreateInfo = vk::FenceCreateInfo().setFlags(vk::FenceCreateFlagBits::eSignaled);
        auto semaphoreCreateInfo = vk::SemaphoreCreateInfo();

        for (std::uint32_t idx = 0; idx < INFLIGHT_FRAME_COUNT; idx++) {
            this->_fences[idx] = this->_deviceContext->getLogicalDevice().createFence(fenceCreateInfo);
            this->_imageReadySemaphores[idx] = this->_deviceContext->getLogicalDevice()
                    .createSemaphore(semaphoreCreateInfo);
            this->_renderFinishedSemaphores[idx] = this->_deviceContext->getLogicalDevice()
                    .createSemaphore(semaphoreCreateInfo);
        }

        this->_eventHandlerIdx = this->_eventQueue->addHandler([this](const Event &event) {
            switch (event.type) {
                case EventType::RenderGraphModified:
                    this->_renderGraphModified = true;
                    break;

                case EventType::SurfaceResized:
                    this->_swapchainModified = true;
                    break;

                default:
                    break;
            }
        });
    }

    void RenderManager::destroy() {
        this->_eventQueue->removeHandler(this->_eventHandlerIdx);

        this->_currentRenderGraphExecutor = std::nullopt;

        this->_deviceContext->getLogicalDevice().free(this->_deviceContext->getCommandPool(), this->_commandBuffers);

        for (std::uint32_t frameIdx = 0; frameIdx < INFLIGHT_FRAME_COUNT; frameIdx++) {
            this->_deviceContext->getLogicalDevice().destroy(this->_fences.at(frameIdx));
            this->_deviceContext->getLogicalDevice().destroy(this->_imageReadySemaphores.at(frameIdx));
            this->_deviceContext->getLogicalDevice().destroy(this->_renderFinishedSemaphores.at(frameIdx));
        }
    }

    void RenderManager::run() {
        this->_thread = std::jthread([this](const std::stop_token &stopToken) {
            bool swapchainInvalid = false;

            while (!stopToken.stop_requested()) {
                if (swapchainInvalid || this->_swapchainModified || this->_renderGraphModified) {
                    this->_deviceContext->getLogicalDevice().waitIdle();
                }

                if (this->_renderGraphModified) {
                    auto newRenderGraph = this->_renderContext->getRenderGraph();

                    if (newRenderGraph.has_value()) {
                        auto newExecutor = this->_renderGraphExecutorProvider->createFor(*newRenderGraph);
                        newExecutor->createFramebuffers();

                        this->_currentRenderGraphExecutor = std::unique_ptr<RenderGraphExecutor>(newExecutor);
                    } else {
                        this->_currentRenderGraphExecutor = std::nullopt;
                    }

                    this->_renderGraphModified = false;
                }

                if (swapchainInvalid || this->_swapchainModified) {
                    if (this->_currentRenderGraphExecutor.has_value()) {
                        (*this->_currentRenderGraphExecutor)->destroyFramebuffers();
                    }

                    this->_presentContext->recreate();

                    if (this->_currentRenderGraphExecutor.has_value()) {
                        (*this->_currentRenderGraphExecutor)->createFramebuffers();
                    }

                    swapchainInvalid = false;
                    this->_swapchainModified = false;
                }

                try {
                    swapchainInvalid = this->renderFrame(this->_frameIdx);

                    this->_frameIdx = (this->_frameIdx + 1) % INFLIGHT_FRAME_COUNT;
                } catch (const vk::OutOfDateKHRError &error) {
                    swapchainInvalid = true;
                }
            }
        });
    }

    void RenderManager::stop() {
        if (this->_thread.has_value()) {
            this->_thread->request_stop();

            if (this->_thread->joinable()) {
                this->_thread->join();
            }

            this->_thread = std::nullopt;
        }

        this->_deviceContext->getLogicalDevice().waitIdle();
    }

    bool RenderManager::renderFrame(const std::uint32_t &frameIdx) const {
        auto logicalDevice = this->_deviceContext->getLogicalDevice();
        auto graphicsQueue = this->_deviceContext->getGraphicsQueue();
        auto presentQueue = this->_deviceContext->getPresentQueue();
        auto swapchain = this->_presentContext->getSwapchain();

        auto commandBuffer = this->_commandBuffers.at(frameIdx);
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

        commandBuffer.reset();

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
