#ifndef PENROSE_UI_WIDGETS_INPUT_HPP
#define PENROSE_UI_WIDGETS_INPUT_HPP

#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class Input : public Widget {
    public:
        struct Args {
            BooleanValue enabled;
            BooleanValue visible;
            StringValue text;
        };

        explicit Input(Args &&args)
                : Widget(std::forward<decltype(args.enabled)>(args.enabled),
                         std::forward<decltype(args.visible)>(args.visible)),
                  _text(std::forward<decltype(args.text)>(args.text)) {
            //
        }

        [[nodiscard]] WidgetType getType() const override { return WidgetType::Input; }

        [[nodiscard]] const StringValue &getText() const { return this->_text; }

    private:
        StringValue _text;
    };
}

#endif // PENROSE_UI_WIDGETS_INPUT_HPP
