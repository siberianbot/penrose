#ifndef PENROSE_UI_WIDGETS_BUTTON_HPP
#define PENROSE_UI_WIDGETS_BUTTON_HPP

#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class Button : public Widget {
    public:
        struct Args {
            BooleanValue enabled;
            BooleanValue visible;
            StringValue title;
            ActionValue action;
        };

        explicit Button(Args &&args)
                : Widget(std::forward<decltype(args.enabled)>(args.enabled),
                         std::forward<decltype(args.visible)>(args.visible)),
                  _title(std::forward<decltype(args.title)>(args.title)),
                  _action(std::forward<decltype(args.action)>(args.action)) {
            //
        }

        [[nodiscard]] WidgetType getType() const override { return WidgetType::Button; }

        [[nodiscard]] const StringValue &getTitle() const { return this->_title; }

        [[nodiscard]] const ActionValue &getAction() const { return this->_action; }

    private:
        StringValue _title;
        ActionValue _action;
    };
}

#endif // PENROSE_UI_WIDGETS_BUTTON_HPP
