#ifndef PENROSE_BUILTIN_DEBUG_RENDERING_DEBUG_UI_DRAW_RENDER_OPERATOR_HPP
#define PENROSE_BUILTIN_DEBUG_RENDERING_DEBUG_UI_DRAW_RENDER_OPERATOR_HPP

#include <string_view>

#include <Penrose/Rendering/RenderOperator.hpp>

namespace Penrose {

    class DebugUIDrawRenderOperator : public RenderOperator {
    public:
        constexpr static const std::string_view NAME = "DebugUIDraw";

        ~DebugUIDrawRenderOperator() override = default;

        [[nodiscard]] std::string getName() const override { return std::string(NAME); }

        [[nodiscard]] ParamsCollection getDefaults() const override { return ParamsCollection::empty(); }
    };
}

#endif // PENROSE_BUILTIN_DEBUG_RENDERING_DEBUG_UI_DRAW_RENDER_OPERATOR_HPP
