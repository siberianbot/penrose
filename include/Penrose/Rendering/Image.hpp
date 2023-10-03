#ifndef PENROSE_RENDERING_IMAGE_HPP
#define PENROSE_RENDERING_IMAGE_HPP

#include <cstdint>

#include <Penrose/Rendering/ImageFormat.hpp>

namespace Penrose {

    class Image {
    public:
        virtual ~Image() = default;

        [[nodiscard]] virtual ImageFormat getFormat() const = 0;
        [[nodiscard]] virtual std::uint32_t getWidth() const = 0;
        [[nodiscard]] virtual std::uint32_t getHeight() const = 0;
    };
}

#endif // PENROSE_RENDERING_IMAGE_HPP
