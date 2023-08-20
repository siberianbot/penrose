#ifndef PENROSE_BUILTIN_RENDERING_VK_RENDER_TARGET_FACTORY_HPP
#define PENROSE_BUILTIN_RENDERING_VK_RENDER_TARGET_FACTORY_HPP

#include <Penrose/Rendering/RenderTargetFactory.hpp>

namespace Penrose {

    class ResourceSet;
    class DeviceContext;
    class PresentContext;

    class VkRenderTargetFactory : public RenderTargetFactory {
    public:
        explicit VkRenderTargetFactory(ResourceSet *resources);
        ~VkRenderTargetFactory() override = default;

        [[nodiscard]] RenderTarget *makeRenderTarget(const RenderTargetInfo &targetInfo) override;

    private:
        DeviceContext *_deviceContext;
        PresentContext *_presentContext;

        [[nodiscard]] RenderTarget *makeImageRenderTarget(const RenderTargetInfo &targetInfo);
        [[nodiscard]] RenderTarget *makeSwapchainRenderTarget(const RenderTargetInfo &targetInfo);
    };
}

#endif // PENROSE_BUILTIN_RENDERING_VK_RENDER_TARGET_FACTORY_HPP
