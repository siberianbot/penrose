#ifndef PENROSE_UI_WIDGETS_CHECKBOX_HPP
#define PENROSE_UI_WIDGETS_CHECKBOX_HPP

#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class Checkbox final: public Widget {
    public:
        struct Args {
            BooleanProperty enabled;
            BooleanProperty visible;
            StringProperty text;
            BooleanProperty checked;
        };

        explicit Checkbox(Args &&args)
            : Widget(
                  std::forward<decltype(args.enabled)>(args.enabled),
                  std::forward<decltype(args.visible)>(args.visible)
              ),
              _text(std::forward<decltype(args.text)>(args.text)),
              _checked(std::forward<decltype(args.checked)>(args.checked)) {
            //
        }

        [[nodiscard]] WidgetType getType() const override { return WidgetType::Checkbox; }

        [[nodiscard]] const StringProperty &getText() const { return this->_text; }

        [[nodiscard]] const BooleanProperty &getChecked() const { return this->_checked; }

    private:
        StringProperty _text;
        BooleanProperty _checked;
    };
}

#endif // PENROSE_UI_WIDGETS_CHECKBOX_HPP
