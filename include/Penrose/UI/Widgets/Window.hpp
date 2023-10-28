#ifndef PENROSE_UI_WIDGETS_WINDOW_HPP
#define PENROSE_UI_WIDGETS_WINDOW_HPP

#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class Window : public Widget<Window, WidgetType::Window> {
    public:
        struct Args {
            BooleanValue visible;
            StringValue title;
        };

        explicit Window(Args &&args)
                : Widget<Window, WidgetType::Window>(std::forward<decltype(args.visible)>(args.visible)),
                  _title(args.title) {
            //
        }

        [[nodiscard]] const StringValue &getTitle() const { return this->_title; }

    private:
        StringValue _title;
    };
}

#endif // PENROSE_UI_WIDGETS_WINDOW_HPP
