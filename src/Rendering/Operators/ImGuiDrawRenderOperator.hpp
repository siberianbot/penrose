#ifndef PENROSE_RENDERING_OPERATORS_IMGUI_DRAW_RENDER_OPERATOR_HPP
#define PENROSE_RENDERING_OPERATORS_IMGUI_DRAW_RENDER_OPERATOR_HPP

#include <Penrose/Rendering/RenderOperator.hpp>

namespace Penrose {

    class ImGuiDrawRenderOperator : public RenderOperator {
    public:
        ~ImGuiDrawRenderOperator() override;

        void execute(const RenderOperatorExecutionContext &context) override;

        [[nodiscard]] static std::string name() { return "ImGuiDraw"; }

        [[nodiscard]] static std::unique_ptr<RenderOperator> create(const RenderOperatorCreateContext &context);
    };
}

#endif // PENROSE_RENDERING_OPERATORS_IMGUI_DRAW_RENDER_OPERATOR_HPP
