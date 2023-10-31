#ifndef PENROSE_UI_WIDGETS_CHECKBOX_HPP
#define PENROSE_UI_WIDGETS_CHECKBOX_HPP

#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class Checkbox : public Widget {
    public:
        struct Args {
            BooleanValue enabled;
            BooleanValue visible;
            StringValue text;
            BooleanValue checked;
        };

        explicit Checkbox(Args &&args)
                : Widget(std::forward<decltype(args.enabled)>(args.enabled),
                         std::forward<decltype(args.visible)>(args.visible)),
                  _text(std::forward<decltype(args.text)>(args.text)),
                  _checked(std::forward<decltype(args.checked)>(args.checked)) {
            //
        }

        [[nodiscard]] WidgetType getType() const override { return WidgetType::Checkbox; }

        [[nodiscard]] const StringValue &getText() const { return this->_text; }

        [[nodiscard]] const BooleanValue &getChecked() const { return this->_checked; }

    private:
        StringValue _text;
        BooleanValue _checked;
    };
}

#endif // PENROSE_UI_WIDGETS_CHECKBOX_HPP
