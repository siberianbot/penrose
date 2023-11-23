#ifndef PENROSE_UI_WIDGETS_MENU_BAR_HPP
#define PENROSE_UI_WIDGETS_MENU_BAR_HPP

#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class MenuBar final: public Widget {
    public:
        struct Args {
            BooleanProperty enabled;
            BooleanProperty visible;
            WidgetList children;
        };

        explicit MenuBar(Args &&args)
            : Widget(
                  std::forward<decltype(args.enabled)>(args.enabled),
                  std::forward<decltype(args.visible)>(args.visible)
              ),
              _children(std::forward<decltype(args.children)>(args.children)) {
            //
        }

        ~MenuBar() override = default;

        [[nodiscard]] WidgetType getType() const override { return WidgetType::MenuBar; }

        [[nodiscard]] const WidgetList &getChildren() const { return this->_children; }

    private:
        WidgetList _children;
    };
}

#endif // PENROSE_UI_WIDGETS_MENU_BAR_HPP
