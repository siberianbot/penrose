#include "UILayoutLoader.hpp"

#include <Penrose/Assets/UILayoutAsset.hpp>

#include "src/Assets/Structs.hpp"

namespace Penrose {

    UILayoutLoader::UILayoutLoader(ResourceSet *resources)
        : _layoutFactory(resources->get<LayoutFactory>()) {
        //
    }

    Asset *UILayoutLoader::fromReader(AssetReader &reader) {
        const auto [size] = reader.read<UILayoutInfo>();

        auto rawData = std::vector<unsigned char>(size);
        reader.read(size, rawData.data());

        const auto layout = this->_layoutFactory->makeLayout(std::forward<decltype(rawData)>(rawData));

        return new UILayoutAsset(std::shared_ptr<Layout>(layout));
    }
}
