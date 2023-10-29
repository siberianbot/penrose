#ifndef PENROSE_UI_WIDGETS_BUTTON_HPP
#define PENROSE_UI_WIDGETS_BUTTON_HPP

#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class Button : public Widget {
    public:
        struct Args {
            BooleanWidgetValue enabled;
            BooleanWidgetValue visible;
            StringWidgetValue title;
            ActionWidgetValue action;
        };

        explicit Button(Args &&args)
                : Widget(std::forward<decltype(args.enabled)>(args.enabled),
                         std::forward<decltype(args.visible)>(args.visible)),
                  _title(std::forward<decltype(args.title)>(args.title)),
                  _action(std::forward<decltype(args.action)>(args.action)) {
            //
        }

        [[nodiscard]] WidgetType getType() const override { return WidgetType::Button; }

        [[nodiscard]] const StringWidgetValue &getTitle() const { return this->_title; }

        [[nodiscard]] const ActionWidgetValue &getAction() const { return this->_action; }

    private:
        StringWidgetValue _title;
        ActionWidgetValue _action;
    };
}

#endif // PENROSE_UI_WIDGETS_BUTTON_HPP
