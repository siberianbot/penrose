#ifndef PENROSE_ASSETS_UI_LAYOUT_ASSET_HPP
#define PENROSE_ASSETS_UI_LAYOUT_ASSET_HPP

#include <memory>

#include <Penrose/Assets/Asset.hpp>
#include <Penrose/UI/Layout.hpp>

namespace Penrose {

    class UILayoutAsset : public Asset {
    public:
        explicit UILayoutAsset(std::unique_ptr<Layout> &&layout);
        ~UILayoutAsset() override = default;

        [[nodiscard]] AssetType getType() const override { return AssetType::UILayout; }

        [[nodiscard]] const Layout *getLayout() const { return this->_layout.get(); }

    private:
        std::unique_ptr<Layout> _layout;
    };
}

#endif // PENROSE_ASSETS_UI_LAYOUT_ASSET_HPP
