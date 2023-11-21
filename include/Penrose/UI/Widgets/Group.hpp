#ifndef PENROSE_UI_WIDGETS_GROUP_HPP
#define PENROSE_UI_WIDGETS_GROUP_HPP

#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class Group final: public Widget {
    public:
        struct Args {
            BooleanProperty enabled;
            BooleanProperty visible;
            std::optional<ObjectProperty> context;
            WidgetList children;
        };

        explicit Group(Args &&args)
            : Widget(
                  std::forward<decltype(args.enabled)>(args.enabled),
                  std::forward<decltype(args.visible)>(args.visible)
              ),
              _context(std::forward<decltype(args.context)>(args.context)),
              _children(std::forward<decltype(args.children)>(args.children)) {
            //
        }

        [[nodiscard]] WidgetType getType() const override { return WidgetType::Group; }

        [[nodiscard]] const std::optional<ObjectProperty> &getContext() const { return this->_context; }

        [[nodiscard]] const WidgetList &getChildren() const { return this->_children; }

    private:
        std::optional<ObjectProperty> _context;
        WidgetList _children;
    };
}

#endif // PENROSE_UI_WIDGETS_GROUP_HPP
