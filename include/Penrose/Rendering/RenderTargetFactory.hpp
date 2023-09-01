#ifndef PENROSE_RENDERING_RENDER_TARGET_FACTORY_HPP
#define PENROSE_RENDERING_RENDER_TARGET_FACTORY_HPP

#include <Penrose/Rendering/RenderTargetInfo.hpp>

namespace Penrose {

    class RenderTarget;

    class RenderTargetFactory {
    public:
        virtual ~RenderTargetFactory() = default;

        [[nodiscard]] virtual RenderTarget *makeRenderTarget(const RenderTargetInfo &targetInfo) = 0;
    };
}

#endif // PENROSE_RENDERING_RENDER_TARGET_FACTORY_HPP
