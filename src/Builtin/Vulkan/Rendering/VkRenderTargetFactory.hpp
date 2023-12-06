#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_TARGET_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_TARGET_FACTORY_HPP

#include <Penrose/Rendering/RenderTargetInfo.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkMemoryAllocator.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderTarget.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainManager.hpp"

namespace Penrose {

    class VkRenderTargetFactory : public Resource<VkRenderTargetFactory> {
    public:
        explicit VkRenderTargetFactory(const ResourceSet *resources);
        ~VkRenderTargetFactory() override = default;

        [[nodiscard]] VkRenderTarget *makeRenderTarget(RenderTargetInfo &&targetInfo);

    private:
        ResourceProxy<VkLogicalDeviceContext> _logicalDeviceContext;
        ResourceProxy<VkMemoryAllocator> _memoryAllocator;
        ResourceProxy<VkSwapchainManager> _swapchainManager;

        [[nodiscard]] VkRenderTarget *makeImageRenderTarget(RenderTargetInfo &&targetInfo);
        [[nodiscard]] VkRenderTarget *makeSwapchainRenderTarget(RenderTargetInfo &&targetInfo);
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_TARGET_FACTORY_HPP
