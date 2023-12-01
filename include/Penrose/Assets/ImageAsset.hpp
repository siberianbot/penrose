#ifndef PENROSE_ASSETS_IMAGE_ASSET_HPP
#define PENROSE_ASSETS_IMAGE_ASSET_HPP

#include <memory>

#include <Penrose/Assets/Asset.hpp>
#include <Penrose/Rendering/Image.hpp>

namespace Penrose {

    /**
     * \brief Image asset
     */
    class PENROSE_API ImageAsset final: public Asset {
    public:
        explicit ImageAsset(std::shared_ptr<Image> &&image)
            : _image(std::forward<decltype(image)>(image)) {
            //
        }

        ~ImageAsset() override = default;

        //! \copydoc Asset::getType
        [[nodiscard]] AssetType getType() const override { return AssetType::Image; }

        /**
         * \brief Get pointer to instance of Image
         * \return instance of Image
         */
        [[nodiscard]] std::weak_ptr<Image> getImage() const { return this->_image; }

    private:
        std::shared_ptr<Image> _image;
    };
}

#endif // PENROSE_ASSETS_IMAGE_ASSET_HPP
