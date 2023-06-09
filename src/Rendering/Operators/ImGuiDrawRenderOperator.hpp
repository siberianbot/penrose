#ifndef PENROSE_RENDERING_OPERATORS_IMGUI_DRAW_RENDER_OPERATOR_HPP
#define PENROSE_RENDERING_OPERATORS_IMGUI_DRAW_RENDER_OPERATOR_HPP

#include <string_view>

#include "src/Rendering/Operators/RenderOperator.hpp"

namespace Penrose {

    static constexpr const std::string_view IMGUI_DRAW_RENDER_OPERATOR_NAME = "ImGuiDraw";

    class ImGuiDrawRenderOperator : public RenderOperator {
    public:
        ~ImGuiDrawRenderOperator() override;

        void execute(const RenderOperatorExecutionContext &context) override;
    };

    class ImGuiDrawRenderOperatorProducer : public RenderOperatorProducer {
    public:
        ~ImGuiDrawRenderOperatorProducer() override = default;

        [[nodiscard]] std::unique_ptr<RenderOperator> produce(
                const RenderOperatorProduceContext &context) const override;
    };
}

#endif // PENROSE_RENDERING_OPERATORS_IMGUI_DRAW_RENDER_OPERATOR_HPP
