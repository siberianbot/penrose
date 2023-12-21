#ifndef PENROSE_ASSETS_LOADERS_IMAGE_LOADER_HPP
#define PENROSE_ASSETS_LOADERS_IMAGE_LOADER_HPP

#include <Penrose/Rendering/Objects/ImageFactory.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Assets/Loaders/TypedAssetLoader.hpp"

namespace Penrose {

    class ImageLoader final: public Resource<ImageLoader>,
                             public TypedAssetLoader {
    public:
        explicit ImageLoader(const ResourceSet *resources);
        ~ImageLoader() override = default;

        [[nodiscard]] AssetType getAssetType() const override { return AssetType::Image; }

        [[nodiscard]] Asset *fromReader(AssetReader &reader) override;

    private:
        ResourceProxy<ImageFactory> _imageFactory;
    };
}

#endif // PENROSE_ASSETS_LOADERS_IMAGE_LOADER_HPP
