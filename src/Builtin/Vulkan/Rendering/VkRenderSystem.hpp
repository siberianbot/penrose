#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SYSTEM_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SYSTEM_HPP

#include <array>
#include <cstdint>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/RenderSystem.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Constants.hpp"
#include "src/Builtin/Vulkan/Rendering/VkCommandManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkDescriptorPoolManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkPhysicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkPipelineFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderGraphContextManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderGraphExecutor.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainManager.hpp"

namespace Penrose {

    class VkRenderSystem final: public Resource<VkRenderSystem>,
                                public RenderSystem {
    public:
        explicit VkRenderSystem(const ResourceSet *resources);
        ~VkRenderSystem() override = default;

        [[nodiscard]] std::string getName() const override { return "Vulkan"; }

        void init() override;
        void destroy() override;

        void render() override;
        void resize() override;

    private:
        ResourceProxy<VkPhysicalDeviceContext> _physicalDeviceContext;
        ResourceProxy<VkLogicalDeviceContext> _logicalDeviceContext;
        ResourceProxy<VkCommandManager> _commandManager;
        ResourceProxy<VkDescriptorPoolManager> _descriptorPoolManager;
        ResourceProxy<VkSwapchainManager> _swapchainManager;
        ResourceProxy<VkPipelineFactory> _pipelineFactory;
        ResourceProxy<VkRenderGraphContextManager> _renderGraphContextManager;
        ResourceProxy<VkRenderGraphExecutor> _renderGraphExecutor;

        std::uint32_t _frameIdx = 0;
        std::array<vk::Fence, INFLIGHT_FRAME_COUNT> _fences;
        std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> _imageReadySemaphores;
        std::array<vk::Semaphore, INFLIGHT_FRAME_COUNT> _renderFinishedSemaphores;

        [[nodiscard]] bool renderFrame(const std::uint32_t &frameIdx);
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_SYSTEM_HPP
