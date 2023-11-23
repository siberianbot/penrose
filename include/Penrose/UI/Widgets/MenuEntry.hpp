#ifndef PENROSE_UI_WIDGETS_MENU_ENTRY_HPP
#define PENROSE_UI_WIDGETS_MENU_ENTRY_HPP

#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class MenuEntry final: public Widget {
    public:
        struct Args {
            BooleanProperty enabled;
            BooleanProperty visible;
            StringProperty title;
            BooleanProperty checked;
            std::optional<ActionProperty> action;
        };

        explicit MenuEntry(Args &&args)
            : Widget(
                  std::forward<decltype(args.enabled)>(args.enabled),
                  std::forward<decltype(args.visible)>(args.visible)
              ),
              _title(std::forward<decltype(args.title)>(args.title)),
              _checked(std::forward<decltype(args.checked)>(args.checked)),
              _action(std::forward<decltype(args.action)>(args.action)) {
            //
        }

        ~MenuEntry() override = default;

        [[nodiscard]] WidgetType getType() const override { return WidgetType::MenuEntry; }

        [[nodiscard]] const StringProperty &getTitle() const { return this->_title; }

        [[nodiscard]] const BooleanProperty &getChecked() const { return this->_checked; }

        [[nodiscard]] const std::optional<ActionProperty> &getAction() const { return this->_action; }

    private:
        StringProperty _title;
        BooleanProperty _checked;
        std::optional<ActionProperty> _action;
    };
}

#endif // PENROSE_UI_WIDGETS_MENU_ENTRY_HPP
