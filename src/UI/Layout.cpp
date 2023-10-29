#include <Penrose/UI/Layout.hpp>

namespace Penrose {

    Layout::Layout(std::unique_ptr<Widget> &&root)
            : _root(std::forward<decltype(root)>(root)) {
        //
    }
}
