#ifndef PENROSE_ASSETS_IMAGE_ASSET_HPP
#define PENROSE_ASSETS_IMAGE_ASSET_HPP

#include <memory>

#include <Penrose/Assets/Asset.hpp>
#include <Penrose/Rendering/Image.hpp>

namespace Penrose {

    class ImageAsset : public Asset {
    public:
        explicit ImageAsset(std::unique_ptr<Image> image);
        ~ImageAsset() override = default;

        [[nodiscard]] AssetType getType() const override { return AssetType::Image; }

        [[nodiscard]] Image *getImage() const { return this->_image.get(); }

    private:
        std::unique_ptr<Image> _image;
    };
}

#endif // PENROSE_ASSETS_IMAGE_ASSET_HPP
