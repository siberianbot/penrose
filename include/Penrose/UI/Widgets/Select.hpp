#ifndef PENROSE_UI_WIDGETS_SELECT_HPP
#define PENROSE_UI_WIDGETS_SELECT_HPP

#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class Select final: public Widget {
    public:
        struct Args {
            BooleanProperty enabled;
            BooleanProperty visible;
            ListProperty items;
            IntegerProperty selection;
        };

        explicit Select(Args &&args)
            : Widget(
                  std::forward<decltype(args.enabled)>(args.enabled),
                  std::forward<decltype(args.visible)>(args.visible)
              ),
              _items(std::forward<decltype(args.items)>(args.items)),
              _selection(std::forward<decltype(args.selection)>(args.selection)) {
            //
        }

        [[nodiscard]] WidgetType getType() const override { return WidgetType::Select; }

        [[nodiscard]] const ListProperty &getItems() const { return this->_items; }

        [[nodiscard]] const IntegerProperty &getSelection() const { return this->_selection; }

    private:
        ListProperty _items;
        IntegerProperty _selection;
    };
}

#endif // PENROSE_UI_WIDGETS_SELECT_HPP
