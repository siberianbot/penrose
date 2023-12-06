#ifndef PENROSE_ASSETS_LOADERS_UI_LAYOUT_LOADER_HPP
#define PENROSE_ASSETS_LOADERS_UI_LAYOUT_LOADER_HPP

#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/UI/LayoutFactory.hpp>

#include "src/Assets/Loaders/TypedAssetLoader.hpp"

namespace Penrose {

    class UILayoutLoader final: public Resource<UILayoutLoader>,
                                public TypedAssetLoader {
    public:
        explicit UILayoutLoader(const ResourceSet *resources);
        ~UILayoutLoader() override = default;

        [[nodiscard]] AssetType getAssetType() const override { return AssetType::UILayout; }

        [[nodiscard]] Asset *fromReader(AssetReader &reader) override;

    private:
        ResourceProxy<LayoutFactory> _layoutFactory;
    };
}

#endif // PENROSE_ASSETS_LOADERS_UI_LAYOUT_LOADER_HPP
