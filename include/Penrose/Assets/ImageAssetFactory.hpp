#ifndef PENROSE_ASSETS_IMAGE_ASSET_FACTORY_HPP
#define PENROSE_ASSETS_IMAGE_ASSET_FACTORY_HPP

#include <cstdint>

#include <Penrose/Assets/ImageAsset.hpp>

namespace Penrose {

    class ImageAssetFactory {
    public:
        virtual ~ImageAssetFactory() = default;

        [[nodiscard]] virtual ImageAsset *makeImage(ImageFormat format, std::uint32_t width, std::uint32_t height,
                                                    void *data) = 0;
    };
}

#endif // PENROSE_ASSETS_IMAGE_ASSET_FACTORY_HPP
