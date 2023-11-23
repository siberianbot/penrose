#ifndef PENROSE_UI_WIDGETS_WINDOW_HPP
#define PENROSE_UI_WIDGETS_WINDOW_HPP

#include <optional>

#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class Window final: public Widget {
    public:
        struct Args {
            BooleanProperty enabled;
            BooleanProperty visible;
            StringProperty title;
            std::optional<ObjectProperty> context;
            std::optional<BooleanProperty> opened;
            WidgetList children;
        };

        explicit Window(Args &&args)
            : Widget(
                  std::forward<decltype(args.enabled)>(args.enabled),
                  std::forward<decltype(args.visible)>(args.visible)
              ),
              _title(std::forward<decltype(args.title)>(args.title)),
              _context(std::forward<decltype(args.context)>(args.context)),
              _opened(std::forward<decltype(args.opened)>(args.opened)),
              _children(std::forward<decltype(args.children)>(args.children)) {
            //
        }

        ~Window() override = default;

        [[nodiscard]] WidgetType getType() const override { return WidgetType::Window; }

        [[nodiscard]] const StringProperty &getTitle() const { return this->_title; }

        [[nodiscard]] const std::optional<ObjectProperty> &getContext() const { return this->_context; }

        [[nodiscard]] const std::optional<BooleanProperty> &getOpened() const { return this->_opened; }

        [[nodiscard]] const WidgetList &getChildren() const { return this->_children; }

    private:
        StringProperty _title;
        std::optional<ObjectProperty> _context;
        std::optional<BooleanProperty> _opened;
        WidgetList _children;
    };
}

#endif // PENROSE_UI_WIDGETS_WINDOW_HPP
