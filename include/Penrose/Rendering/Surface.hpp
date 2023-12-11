#ifndef PENROSE_RENDERING_SURFACE_HPP
#define PENROSE_RENDERING_SURFACE_HPP

#include <Penrose/Types/Size.hpp>

namespace Penrose {

    class Surface {
    public:
        virtual ~Surface() = default;

        [[nodiscard]] virtual Size getSize() const = 0;

        virtual void setSize(const Size &size) = 0;

        [[nodiscard]] virtual bool isCursorLocked() const = 0;

        virtual void lockCursor() = 0;
        virtual void unlockCursor() = 0;
    };
}

#endif // PENROSE_RENDERING_SURFACE_HPP
