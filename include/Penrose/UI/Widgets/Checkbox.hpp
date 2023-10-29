#ifndef PENROSE_UI_WIDGETS_CHECKBOX_HPP
#define PENROSE_UI_WIDGETS_CHECKBOX_HPP

#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class Checkbox : public Widget {
    public:
        struct Args {
            BooleanWidgetValue enabled;
            BooleanWidgetValue visible;
            BooleanWidgetValue checked;
        };

        explicit Checkbox(Args &&args)
                : Widget(std::forward<decltype(args.enabled)>(args.enabled),
                         std::forward<decltype(args.visible)>(args.visible)),
                  _checked(std::forward<decltype(args.checked)>(args.checked)) {
            //
        }

        [[nodiscard]] WidgetType getType() const override { return WidgetType::Checkbox; }

        [[nodiscard]] const BooleanWidgetValue &getChecked() const { return this->_checked; }

    private:
        BooleanWidgetValue _checked;
    };
}

#endif // PENROSE_UI_WIDGETS_CHECKBOX_HPP
