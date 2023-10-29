#ifndef PENROSE_UI_WIDGETS_LABEL_HPP
#define PENROSE_UI_WIDGETS_LABEL_HPP

#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class Label : public Widget {
    public:
        struct Args {
            BooleanWidgetValue enabled;
            BooleanWidgetValue visible;
            StringWidgetValue text;
        };

        explicit Label(Args &&args)
                : Widget(std::forward<decltype(args.enabled)>(args.enabled),
                         std::forward<decltype(args.visible)>(args.visible)),
                  _text(std::forward<decltype(args.text)>(args.text)) {
            //
        }

        [[nodiscard]] WidgetType getType() const override { return WidgetType::Label; }

        [[nodiscard]] const StringWidgetValue &getText() const { return this->_text; }

    private:
        StringWidgetValue _text;
    };
}

#endif // PENROSE_UI_WIDGETS_LABEL_HPP
