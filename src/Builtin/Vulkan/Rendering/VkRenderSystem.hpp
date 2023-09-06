#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SYSTEM_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SYSTEM_HPP

#include <array>
#include <cstdint>
#include <optional>
#include <memory>

#include <vulkan/vulkan.hpp>

#include <Penrose/Events/Event.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Rendering/RenderGraphContext.hpp>
#include <Penrose/Rendering/RenderGraphHook.hpp>
#include <Penrose/Rendering/RenderSystem.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/Runnable.hpp>

#include "src/Rendering/RenderGraphExecutor.hpp"
#include "src/Rendering/RenderGraphExecutorProvider.hpp"

#include "src/Builtin/Vulkan/Constants.hpp"
#include "src/Builtin/Vulkan/Rendering/VkCommandManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainManager.hpp"

namespace Penrose {

    class ResourceSet;

    class VkRenderSystem : public Resource,
                           public Initializable,
                           public Runnable,
                           public RenderSystem,
                           public RenderGraphHook {
    public:
        explicit VkRenderSystem(ResourceSet *resources);
        ~VkRenderSystem() override = default;

        void init() override;
        void destroy() override;

        void run() override { /* nothing to do */ }
        void stop() override;

        void renderFrame() override;

        void onRenderGraphModified(const std::optional<RenderGraphInfo> &graphInfo) override;

    private:
        Lazy<VkCommandManager> _commandManager;
        Lazy<EventQueue> _eventQueue;
        Lazy<VkLogicalDeviceContext> _logicalDeviceContext;
        Lazy<RenderGraphContext> _renderGraphContext;
        Lazy<RenderGraphExecutorProvider> _renderGraphExecutorProvider;
        Lazy<VkSwapchainManager> _swapchainManager;

        std::uint32_t _frameIdx = 0;
        std::array<vk::Fence, INFLIGHT_FRAME_COUNT> _fences;
        std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> _imageReadySemaphores;
        std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> _renderFinishedSemaphores;

        EventHandlerIndex _eventHandlerIdx = -1;

        volatile bool _swapchainInvalid = false;
        volatile bool _swapchainModified = false;
        volatile bool _renderGraphModified = false;
        std::optional<std::unique_ptr<RenderGraphExecutor>> _currentRenderGraphExecutor;

        [[nodiscard]] bool renderFrame(const std::uint32_t &frameIdx);
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SYSTEM_HPP
