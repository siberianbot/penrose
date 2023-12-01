#ifndef PENROSE_ASSETS_UI_LAYOUT_ASSET_HPP
#define PENROSE_ASSETS_UI_LAYOUT_ASSET_HPP

#include <memory>

#include <Penrose/Assets/Asset.hpp>
#include <Penrose/UI/Layout.hpp>

namespace Penrose {

    /**
     * \brief UI layout asset
     */
    class PENROSE_API UILayoutAsset final: public Asset {
    public:
        explicit UILayoutAsset(std::shared_ptr<Layout> &&layout)
            : _layout(std::forward<decltype(layout)>(layout)) {
            //
        }

        ~UILayoutAsset() override = default;

        //! \copydoc Asset::getType
        [[nodiscard]] AssetType getType() const override { return AssetType::UILayout; }

        /**
         * \brief Get pointer to instance of Layout
         * \return instance of Layout
         */
        [[nodiscard]] std::weak_ptr<Layout> getLayout() const { return this->_layout; }

    private:
        std::shared_ptr<Layout> _layout;
    };
}

#endif // PENROSE_ASSETS_UI_LAYOUT_ASSET_HPP
