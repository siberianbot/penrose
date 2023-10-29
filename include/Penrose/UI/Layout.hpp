#ifndef PENROSE_UI_LAYOUT_HPP
#define PENROSE_UI_LAYOUT_HPP

#include <list>
#include <memory>

#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class Layout {
    public:
        explicit Layout(std::unique_ptr<Widget> &&root);

        [[nodiscard]] const Widget *getRoot() const { return this->_root.get(); }

    private:
        std::unique_ptr<Widget> _root;
    };
}

#endif // PENROSE_UI_LAYOUT_HPP
