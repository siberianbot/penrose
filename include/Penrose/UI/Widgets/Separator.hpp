#ifndef PENROSE_UI_WIDGETS_SEPARATOR_HPP
#define PENROSE_UI_WIDGETS_SEPARATOR_HPP

#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class Separator final: public Widget {
    public:
        explicit Separator()
            : Widget(BooleanProperty(true), BooleanProperty(true)) {
            //
        }

        ~Separator() override = default;

        [[nodiscard]] WidgetType getType() const override { return WidgetType::Separator; }
    };
}

#endif // PENROSE_UI_WIDGETS_SEPARATOR_HPP
