#ifndef PENROSE_RENDERING_IMAGE_FACTORY_HPP
#define PENROSE_RENDERING_IMAGE_FACTORY_HPP

#include <cstdint>

#include <Penrose/Rendering/Image.hpp>

namespace Penrose {

    class ImageFactory {
    public:
        virtual ~ImageFactory() = default;

        [[nodiscard]] virtual Image *makeImage(ImageFormat format, std::uint32_t width, std::uint32_t height) = 0;
    };
}

#endif // PENROSE_RENDERING_IMAGE_FACTORY_HPP
