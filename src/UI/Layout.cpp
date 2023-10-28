#include <Penrose/UI/Layout.hpp>

namespace Penrose {

    Layout::Layout(std::unique_ptr<WidgetBase> &&root)
            : _root(std::forward<decltype(root)>(root)) {
        //
    }
}
