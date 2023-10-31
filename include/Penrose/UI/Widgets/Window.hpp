#ifndef PENROSE_UI_WIDGETS_WINDOW_HPP
#define PENROSE_UI_WIDGETS_WINDOW_HPP

#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class Window : public Widget {
    public:
        struct Args {
            BooleanValue enabled;
            BooleanValue visible;
            StringValue title;
            ObjectValue context;
            WidgetList children;
        };

        explicit Window(Args &&args)
                : Widget(std::forward<decltype(args.enabled)>(args.enabled),
                         std::forward<decltype(args.visible)>(args.visible)),
                  _title(std::forward<decltype(args.title)>(args.title)),
                  _context(std::forward<decltype(args.context)>(args.context)),
                  _children(std::forward<decltype(args.children)>(args.children)) {
            //
        }

        [[nodiscard]] WidgetType getType() const override { return WidgetType::Window; }

        [[nodiscard]] const StringValue &getTitle() const { return this->_title; }

        [[nodiscard]] const ObjectValue &getContext() const { return this->_context; }

        [[nodiscard]] const WidgetList &getChildren() const { return this->_children; }

    private:
        StringValue _title;
        ObjectValue _context;
        WidgetList _children;
    };
}

#endif // PENROSE_UI_WIDGETS_WINDOW_HPP
