#ifndef PENROSE_UI_WIDGETS_BUTTON_HPP
#define PENROSE_UI_WIDGETS_BUTTON_HPP

#include <optional>

#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class Button final: public Widget {
    public:
        struct Args {
            BooleanProperty enabled;
            BooleanProperty visible;
            StringProperty title;
            std::optional<ActionProperty> action;
        };

        explicit Button(Args &&args)
            : Widget(
                  std::forward<decltype(args.enabled)>(args.enabled),
                  std::forward<decltype(args.visible)>(args.visible)
              ),
              _title(std::forward<decltype(args.title)>(args.title)),
              _action(std::forward<decltype(args.action)>(args.action)) {
            //
        }

        [[nodiscard]] WidgetType getType() const override { return WidgetType::Button; }

        [[nodiscard]] const StringProperty &getTitle() const { return this->_title; }

        [[nodiscard]] const std::optional<ActionProperty> &getAction() const { return this->_action; }

    private:
        StringProperty _title;
        std::optional<ActionProperty> _action;
    };
}

#endif // PENROSE_UI_WIDGETS_BUTTON_HPP
