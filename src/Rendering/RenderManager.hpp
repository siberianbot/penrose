#ifndef PENROSE_RENDERING_RENDER_MANAGER_HPP
#define PENROSE_RENDERING_RENDER_MANAGER_HPP

#include <array>
#include <cstdint>
#include <optional>
#include <memory>
#include <thread>

#include <vulkan/vulkan.hpp>

#include <Penrose/Events/Event.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/Runnable.hpp>

#include "src/Builtin/Vulkan/Constants.hpp"

namespace Penrose {

    class ResourceSet;
    class EventQueue;
    class DeviceContext;
    class PresentContext;
    class RenderContext;
    class RenderGraphExecutor;
    class RenderGraphExecutorProvider;

    class VkLogicalDeviceContext;

    class RenderManager : public Resource, public Initializable, public Runnable {
    public:
        explicit RenderManager(ResourceSet *resources);
        ~RenderManager() override = default;

        void init() override;
        void destroy() override;

        void run() override;
        void stop() override;

    private:
        EventQueue *_eventQueue;
        VkLogicalDeviceContext *_logicalDeviceContext;
        DeviceContext *_deviceContext;
        PresentContext *_presentContext;
        RenderContext *_renderContext;
        RenderGraphExecutorProvider *_renderGraphExecutorProvider;

        std::uint32_t _frameIdx = 0;
        std::array<vk::CommandBuffer, INFLIGHT_FRAME_COUNT> _commandBuffers;
        std::array<vk::Fence, INFLIGHT_FRAME_COUNT> _fences;
        std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> _imageReadySemaphores;
        std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> _renderFinishedSemaphores;

        EventHandlerIndex _eventHandlerIdx = -1;

        volatile bool _swapchainModified = false;
        volatile bool _renderGraphModified = false;
        std::optional<std::jthread> _thread;
        std::optional<std::unique_ptr<RenderGraphExecutor>> _currentRenderGraphExecutor;

        [[nodiscard]] bool renderFrame(const std::uint32_t &frameIdx) const;
    };
}

#endif // PENROSE_RENDERING_RENDER_MANAGER_HPP
