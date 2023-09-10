#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SYSTEM_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SYSTEM_HPP

#include <array>
#include <cstdint>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/RenderSystem.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/Runnable.hpp>

#include "src/Builtin/Vulkan/Constants.hpp"
#include "src/Builtin/Vulkan/Rendering/VkCommandManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderGraphContextManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderGraphExecutor.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainManager.hpp"

namespace Penrose {

    class ResourceSet;

    class VkRenderSystem : public Resource,
                           public Initializable,
                           public Runnable,
                           public RenderSystem {
    public:
        explicit VkRenderSystem(ResourceSet *resources);
        ~VkRenderSystem() override = default;

        void init() override;
        void destroy() override;

        void run() override { /* nothing to do */ }

        void stop() override;

        void renderFrame() override;

    private:
        Lazy<VkCommandManager> _commandManager;
        Lazy<VkLogicalDeviceContext> _logicalDeviceContext;
        Lazy<VkRenderGraphContextManager> _renderGraphContextManager;
        Lazy<VkRenderGraphExecutor> _renderGraphExecutor;
        Lazy<VkSwapchainManager> _swapchainManager;

        std::uint32_t _frameIdx = 0;
        std::array<vk::Fence, INFLIGHT_FRAME_COUNT> _fences;
        std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> _imageReadySemaphores;
        std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> _renderFinishedSemaphores;

        [[nodiscard]] bool renderFrame(const std::uint32_t &frameIdx);
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SYSTEM_HPP
