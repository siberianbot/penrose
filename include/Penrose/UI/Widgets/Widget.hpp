#ifndef PENROSE_UI_WIDGETS_WIDGET_HPP
#define PENROSE_UI_WIDGETS_WIDGET_HPP

#include <string>

#include <Penrose/UI/Value.hpp>

namespace Penrose {

    enum class WidgetType {
        Window
    };

    class WidgetBase {
    public:
        virtual ~WidgetBase() = default;

        [[nodiscard]] virtual WidgetType getType() const = 0;

        [[nodiscard]] virtual const BooleanValue &getVisible() const = 0;
    };

    template<typename W, WidgetType Type>
    class Widget : public WidgetBase {
    public:
        ~Widget() override = default;

        [[nodiscard]] WidgetType getType() const override { return Type; }

        [[nodiscard]] const BooleanValue &getVisible() const override { return this->_visible; }

    protected:
        explicit Widget(BooleanValue &&visible)
                : _visible(visible) {
            //
        }

    private:
        BooleanValue _visible;
    };
}

#endif // PENROSE_UI_WIDGETS_WIDGET_HPP
