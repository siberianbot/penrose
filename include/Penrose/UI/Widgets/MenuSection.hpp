#ifndef PENROSE_UI_WIDGETS_MENU_SECTION_HPP
#define PENROSE_UI_WIDGETS_MENU_SECTION_HPP

#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class MenuSection final: public Widget {
    public:
        struct Args {
            BooleanProperty enabled;
            BooleanProperty visible;
            StringProperty title;
            WidgetList children;
        };

        explicit MenuSection(Args &&args)
            : Widget(
                  std::forward<decltype(args.enabled)>(args.enabled),
                  std::forward<decltype(args.visible)>(args.visible)
              ),
              _title(std::forward<decltype(args.title)>(args.title)),
              _children(std::forward<decltype(args.children)>(args.children)) {
            //
        }

        ~MenuSection() override = default;

        [[nodiscard]] WidgetType getType() const override { return WidgetType::MenuSection; }

        [[nodiscard]] const StringProperty &getTitle() const { return this->_title; }

        [[nodiscard]] const WidgetList &getChildren() const { return this->_children; }

    private:
        StringProperty _title;
        WidgetList _children;
    };
}

#endif // PENROSE_UI_WIDGETS_MENU_SECTION_HPP
