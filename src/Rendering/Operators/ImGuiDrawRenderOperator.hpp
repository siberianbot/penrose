#ifndef PENROSE_RENDERING_OPERATORS_IMGUI_DRAW_RENDER_OPERATOR_HPP
#define PENROSE_RENDERING_OPERATORS_IMGUI_DRAW_RENDER_OPERATOR_HPP

#include <string_view>

#include "src/Rendering/Operators/RenderOperator.hpp"

namespace Penrose {

    static constexpr const std::string_view IMGUI_DRAW_RENDER_OPERATOR_NAME = "ImGuiDraw";

    class ImGuiDrawRenderOperator : public RenderOperator {
    private:
        vk::Device _logicalDevice;
        vk::DescriptorPool _descriptorPool;

    public:
        ImGuiDrawRenderOperator(const vk::Device &logicalDevice,
                                const vk::DescriptorPool &descriptorPool);
        ~ImGuiDrawRenderOperator() override;

        void execute(const vk::CommandBuffer &commandBuffer) override;
    };

    class ImGuiDrawRenderOperatorProducer : public RenderOperatorProducer {
    public:
        ~ImGuiDrawRenderOperatorProducer() override = default;

        [[nodiscard]] std::unique_ptr<RenderOperator> produce(
                const RenderOperatorProduceContext &context) const override;
    };
}

#endif // PENROSE_RENDERING_OPERATORS_IMGUI_DRAW_RENDER_OPERATOR_HPP
