#ifndef PENROSE_RENDERING_UTILS_HPP
#define PENROSE_RENDERING_UTILS_HPP

#include <cstdint>

#include <Penrose/Rendering/Image.hpp>

namespace Penrose {

    [[nodiscard]] std::uint8_t getChannels(ImageFormat format);
}

#endif // PENROSE_RENDERING_UTILS_HPP
