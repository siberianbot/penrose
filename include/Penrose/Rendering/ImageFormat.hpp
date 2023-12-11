#ifndef PENROSE_RENDERING_IMAGE_FORMAT_HPP
#define PENROSE_RENDERING_IMAGE_FORMAT_HPP

#include <cstdint>

namespace Penrose {

    /**
     * \brief Available image formats
     */
    enum class ImageFormat : std::uint8_t {
        Grey = 1,
        GreyAlpha = 2,
        RGB = 3,
        RGBA = 4
    };
}

#endif // PENROSE_RENDERING_IMAGE_FORMAT_HPP
