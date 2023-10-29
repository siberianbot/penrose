#ifndef PENROSE_UI_WIDGETS_WINDOW_HPP
#define PENROSE_UI_WIDGETS_WINDOW_HPP

#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class Window : public Widget {
    public:
        struct Args {
            BooleanWidgetValue enabled;
            BooleanWidgetValue visible;
            StringWidgetValue title;
            WidgetList children;
        };

        explicit Window(Args &&args)
                : Widget(std::forward<decltype(args.enabled)>(args.enabled),
                         std::forward<decltype(args.visible)>(args.visible)),
                  _title(std::forward<decltype(args.title)>(args.title)),
                  _children(std::forward<decltype(args.children)>(args.children)) {
            //
        }

        [[nodiscard]] WidgetType getType() const override { return WidgetType::Window; }

        [[nodiscard]] const StringWidgetValue &getTitle() const { return this->_title; }

    private:
        StringWidgetValue _title;
        WidgetList _children;
    };
}

#endif // PENROSE_UI_WIDGETS_WINDOW_HPP
