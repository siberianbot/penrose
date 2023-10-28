#include <Penrose/Assets/UILayoutAsset.hpp>

namespace Penrose {

    UILayoutAsset::UILayoutAsset(std::unique_ptr<Layout> &&layout)
            : _layout(std::forward<decltype(layout)>(layout)) {
        //
    }
}
