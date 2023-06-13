#ifndef PENROSE_RENDERING_OPERATORS_IMGUI_DRAW_RENDER_OPERATOR_HPP
#define PENROSE_RENDERING_OPERATORS_IMGUI_DRAW_RENDER_OPERATOR_HPP

#include <string_view>

#include "src/Rendering/Operators/RenderOperatorFactory.hpp"

namespace Penrose {

    class ImGuiDrawRenderOperator : public RenderOperator {
    public:
        static constexpr const std::string_view NAME = "ImGuiDraw";

        ~ImGuiDrawRenderOperator() override;

        void execute(const RenderOperatorExecutionContext &context) override;

        [[nodiscard]] static std::unique_ptr<RenderOperator> make(const RenderOperatorFactoryContext &context);
    };
}

#endif // PENROSE_RENDERING_OPERATORS_IMGUI_DRAW_RENDER_OPERATOR_HPP
