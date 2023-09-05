#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_TARGET_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_TARGET_FACTORY_HPP

#include <Penrose/Rendering/RenderTargetFactory.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkMemoryAllocator.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainManager.hpp"

namespace Penrose {

    class ResourceSet;

    class VkRenderTargetFactory : public Resource, public RenderTargetFactory {
    public:
        explicit VkRenderTargetFactory(ResourceSet *resources);
        ~VkRenderTargetFactory() override = default;

        [[nodiscard]] RenderTarget *makeRenderTarget(RenderTargetInfo &&targetInfo) override;

    private:
        Lazy<VkLogicalDeviceContext> _logicalDeviceContext;
        Lazy<VkMemoryAllocator> _memoryAllocator;
        Lazy<VkSwapchainManager> _swapchainManager;

        [[nodiscard]] RenderTarget *makeImageRenderTarget(RenderTargetInfo &&targetInfo);
        [[nodiscard]] RenderTarget *makeSwapchainRenderTarget(RenderTargetInfo &&targetInfo);
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_TARGET_FACTORY_HPP
