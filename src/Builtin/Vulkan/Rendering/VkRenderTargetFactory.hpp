#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_TARGET_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_TARGET_FACTORY_HPP

#include <Penrose/Rendering/RenderTargetInfo.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkMemoryAllocator.hpp"
#include "src/Builtin/Vulkan/Rendering/VkRenderTarget.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainManager.hpp"

namespace Penrose {

    class ResourceSet;

    class VkRenderTargetFactory : public Resource {
    public:
        explicit VkRenderTargetFactory(ResourceSet *resources);
        ~VkRenderTargetFactory() override = default;

        [[nodiscard]] VkRenderTarget *makeRenderTarget(RenderTargetInfo &&targetInfo);

    private:
        Lazy<VkLogicalDeviceContext> _logicalDeviceContext;
        Lazy<VkMemoryAllocator> _memoryAllocator;
        Lazy<VkSwapchainManager> _swapchainManager;

        [[nodiscard]] VkRenderTarget *makeImageRenderTarget(RenderTargetInfo &&targetInfo);
        [[nodiscard]] VkRenderTarget *makeSwapchainRenderTarget(RenderTargetInfo &&targetInfo);
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_TARGET_FACTORY_HPP
