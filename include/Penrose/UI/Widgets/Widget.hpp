#ifndef PENROSE_UI_WIDGETS_WIDGET_HPP
#define PENROSE_UI_WIDGETS_WIDGET_HPP

#include <list>
#include <memory>

#include <Penrose/UI/Property.hpp>

namespace Penrose {

    enum class WidgetType {
        Window,
        Label,
        Button,
        Input,
        Checkbox,
        List,
        Group,
        Select
    };

    class Widget {
    public:
        virtual ~Widget() = default;

        [[nodiscard]] virtual WidgetType getType() const = 0;

        [[nodiscard]] const BooleanProperty &getEnabled() const { return this->_enabled; }

        [[nodiscard]] const BooleanProperty &getVisible() const { return this->_visible; }

    protected:
        Widget(BooleanProperty &&enabled, BooleanProperty &&visible)
            : _enabled(std::forward<decltype(enabled)>(enabled)),
              _visible(std::forward<decltype(visible)>(visible)) {
            //
        }

    private:
        BooleanProperty _enabled;
        BooleanProperty _visible;
    };

    using WidgetInstance = std::unique_ptr<Widget>;
    using WidgetList = std::list<WidgetInstance>;
}

#endif // PENROSE_UI_WIDGETS_WIDGET_HPP
