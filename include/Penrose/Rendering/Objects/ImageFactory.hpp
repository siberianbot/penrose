#ifndef PENROSE_RENDERING_IMAGE_FACTORY_HPP
#define PENROSE_RENDERING_IMAGE_FACTORY_HPP

#include <cstdint>
#include <vector>

#include <Penrose/Rendering/Objects/Image.hpp>

namespace Penrose {

    /**
     * \brief Image object factory interface
     */
    class PENROSE_API ImageFactory {
    public:
        virtual ~ImageFactory() = default;

        /**
         * \brief Create instance of empty image object
         * \param format Format of image
         * \param width Width of image
         * \param height Height of image
         * \return Instance of image object
         */
        [[nodiscard]] virtual Image *makeImage(ImageFormat format, std::uint32_t width, std::uint32_t height) = 0;

        /**
         * \brief Create instance of image object
         * \param format Format of image
         * \param width Width of image
         * \param height Height of image
         * \param rawData Raw image data
         * \return Instance of image object
         */
        [[nodiscard]] virtual Image *makeImage(
            ImageFormat format, std::uint32_t width, std::uint32_t height, std::vector<std::byte> &&rawData
        ) = 0;
    };
}

#endif // PENROSE_RENDERING_IMAGE_FACTORY_HPP
