#ifndef PENROSE_RENDERING_RENDER_TARGET_FACTORY_HPP
#define PENROSE_RENDERING_RENDER_TARGET_FACTORY_HPP

#include <Penrose/Rendering/RenderTargetInfo.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class RenderTarget;

    class RenderTargetFactory : public Resource {
    public:
        ~RenderTargetFactory() override = default;

        [[nodiscard]] virtual RenderTarget *makeRenderTarget(const RenderTargetInfo &targetInfo) = 0;
    };
}

#endif // PENROSE_RENDERING_RENDER_TARGET_FACTORY_HPP
