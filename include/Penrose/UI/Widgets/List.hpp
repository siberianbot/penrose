#ifndef PENROSE_UI_WIDGETS_LIST_HPP
#define PENROSE_UI_WIDGETS_LIST_HPP

#include <optional>

#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class List final: public Widget {
    public:
        struct Args {
            BooleanProperty enabled;
            BooleanProperty visible;
            WidgetInstance itemTemplate;
            ListProperty items;
            std::optional<IntegerProperty> selection;
        };

        explicit List(Args &&args)
            : Widget(
                  std::forward<decltype(args.enabled)>(args.enabled),
                  std::forward<decltype(args.visible)>(args.visible)
              ),
              _itemTemplate(std::forward<decltype(args.itemTemplate)>(args.itemTemplate)),
              _items(std::forward<decltype(args.items)>(args.items)),
              _selection(std::forward<decltype(args.selection)>(args.selection)) {
            //
        }

        [[nodiscard]] WidgetType getType() const override { return WidgetType::List; }

        [[nodiscard]] const WidgetInstance &getItemTemplate() const { return this->_itemTemplate; }

        [[nodiscard]] const ListProperty &getItems() const { return this->_items; }

        [[nodiscard]] const std::optional<IntegerProperty> &getSelection() const { return this->_selection; }

    private:
        WidgetInstance _itemTemplate;
        ListProperty _items;
        std::optional<IntegerProperty> _selection;
    };
}

#endif // PENROSE_UI_WIDGETS_LIST_HPP
