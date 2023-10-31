#ifndef PENROSE_UI_WIDGETS_WIDGET_HPP
#define PENROSE_UI_WIDGETS_WIDGET_HPP

#include <list>
#include <memory>

#include <Penrose/UI/Value.hpp>

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

        [[nodiscard]] const BooleanValue &getEnabled() const { return this->_enabled; }

        [[nodiscard]] const BooleanValue &getVisible() const { return this->_visible; }

    protected:
        Widget(BooleanValue &&enabled, BooleanValue &&visible)
                : _enabled(enabled),
                  _visible(visible) {
            //
        }

    private:
        BooleanValue _enabled;
        BooleanValue _visible;
    };

    using WidgetList = std::list<std::unique_ptr<Widget>>;
}

#endif // PENROSE_UI_WIDGETS_WIDGET_HPP
