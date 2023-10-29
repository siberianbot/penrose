#ifndef PENROSE_UI_WIDGETS_WIDGET_HPP
#define PENROSE_UI_WIDGETS_WIDGET_HPP

#include <list>
#include <memory>

#include <Penrose/UI/WidgetValue.hpp>

namespace Penrose {

    enum class WidgetType {
        Window,
        Label,
        Button,
        Input,
        Checkbox
    };

    class Widget {
    public:
        virtual ~Widget() = default;

        [[nodiscard]] virtual WidgetType getType() const = 0;

        [[nodiscard]] const BooleanWidgetValue &getEnabled() const { return this->_enabled; }

        [[nodiscard]] const BooleanWidgetValue &getVisible() const { return this->_visible; }

    protected:
        Widget(BooleanWidgetValue &&enabled, BooleanWidgetValue &&visible)
                : _enabled(enabled),
                  _visible(visible) {
            //
        }

    private:
        BooleanWidgetValue _enabled;
        BooleanWidgetValue _visible;
    };

    using WidgetList = std::list<std::unique_ptr<Widget>>;
}

#endif // PENROSE_UI_WIDGETS_WIDGET_HPP
