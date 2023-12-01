#include "ImageLoader.hpp"

#include <Penrose/Assets/ImageAsset.hpp>

#include "src/Assets/Structs.hpp"

namespace Penrose {

    ImageLoader::ImageLoader(ResourceSet *resources)
        : _renderingObjectManager(resources->get<RenderingObjectManager>()) {
        //
    }

    Asset *ImageLoader::fromReader(AssetReader &reader) {
        const auto [width, height, format, size] = reader.read<ImageInfo>();

        auto rawData = std::vector<std::byte>(size);
        reader.read(size, rawData.data());

        const auto image = this->_renderingObjectManager->makeImage(
            format, width, height, std::forward<decltype(rawData)>(rawData)
        );

        return new ImageAsset(std::shared_ptr<Image>(image));
    }
}
