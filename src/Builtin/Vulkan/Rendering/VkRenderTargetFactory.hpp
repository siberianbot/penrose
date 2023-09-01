#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_TARGET_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_TARGET_FACTORY_HPP

#include <Penrose/Rendering/RenderTargetFactory.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/PresentContext.hpp"

namespace Penrose {

    class ResourceSet;

    class VkRenderTargetFactory : public Resource, public RenderTargetFactory {
    public:
        explicit VkRenderTargetFactory(ResourceSet *resources);
        ~VkRenderTargetFactory() override = default;

        [[nodiscard]] RenderTarget *makeRenderTarget(const RenderTargetInfo &targetInfo) override;

    private:
        Lazy<DeviceContext> _deviceContext;
        Lazy<PresentContext> _presentContext;

        [[nodiscard]] RenderTarget *makeImageRenderTarget(const RenderTargetInfo &targetInfo);
        [[nodiscard]] RenderTarget *makeSwapchainRenderTarget(const RenderTargetInfo &targetInfo);
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_TARGET_FACTORY_HPP
