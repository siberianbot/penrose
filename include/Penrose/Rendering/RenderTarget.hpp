#ifndef PENROSE_RENDERING_RENDER_TARGET_HPP
#define PENROSE_RENDERING_RENDER_TARGET_HPP

#include <Penrose/Rendering/RenderTargetInfo.hpp>

namespace Penrose {

    class RenderTarget {
    public:
        virtual ~RenderTarget() = default;

        [[nodiscard]] virtual const RenderTargetInfo &getTargetInfo() const = 0;
    };
}

#endif // PENROSE_RENDERING_RENDER_TARGET_HPP
