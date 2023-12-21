#ifndef PENROSE_RENDERING_OBJECTS_IMAGE_HPP
#define PENROSE_RENDERING_OBJECTS_IMAGE_HPP

#include <cstdint>

#include <Penrose/Rendering/ImageFormat.hpp>

namespace Penrose {

    /**
     * \brief Image object
     * \details Image objects represents allocated in GPU image.
     */
    class PENROSE_API Image {
    public:
        virtual ~Image() = default;

        /**
         * \brief Get format of image
         * \return Format of image
         */
        [[nodiscard]] virtual ImageFormat getFormat() const = 0;

        /**
         * \brief Get width of image
         * \return Width of image
         */
        [[nodiscard]] virtual std::uint32_t getWidth() const = 0;

        /**
         * \brief Get height of image
         * \return Height of image
         */
        [[nodiscard]] virtual std::uint32_t getHeight() const = 0;
    };
}

#endif // PENROSE_RENDERING_OBJECTS_IMAGE_HPP
