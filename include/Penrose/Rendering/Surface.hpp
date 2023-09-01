#ifndef PENROSE_RENDERING_SURFACE_HPP
#define PENROSE_RENDERING_SURFACE_HPP

#include <Penrose/Common/Size.hpp>

namespace Penrose {

    class Surface {
    public:
        virtual ~Surface() = default;

        [[nodiscard]] virtual Size getSize() const = 0;
    };
}

#endif // PENROSE_RENDERING_SURFACE_HPP
