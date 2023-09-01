#ifndef PENROSE_RENDERING_SURFACE_FACTORY_HPP
#define PENROSE_RENDERING_SURFACE_FACTORY_HPP

#include <Penrose/Rendering/Surface.hpp>

namespace Penrose {

    class SurfaceFactory {
    public:
        virtual ~SurfaceFactory() = default;

        [[nodiscard]] virtual Surface *makeSurface() = 0;
    };
}

#endif // PENROSE_RENDERING_SURFACE_FACTORY_HPP
