#ifndef PENROSE_BUILTIN_RENDERING_IMGUI_DRAW_RENDER_OPERATOR_HPP
#define PENROSE_BUILTIN_RENDERING_IMGUI_DRAW_RENDER_OPERATOR_HPP

#include <string_view>

#include <Penrose/Rendering/RenderOperator.hpp>
#include <Penrose/Rendering/RenderOperatorFactory.hpp>

namespace Penrose {

    class ResourceSet;
    class VulkanBackend;
    class DeviceContext;
    class PresentContext;

    class ImGuiDrawRenderOperator : public RenderOperator {
    public:
        static constexpr const std::string_view NAME = "ImGuiDraw";

        ~ImGuiDrawRenderOperator() override;

        void execute(const RenderOperator::Context &context) override;
    };

    class ImGuiDrawRenderOperatorFactory : public RenderOperatorFactory {
    public:
        explicit ImGuiDrawRenderOperatorFactory(ResourceSet *resources);
        ~ImGuiDrawRenderOperatorFactory() override = default;

        [[nodiscard]] std::string_view name() const override {
            return ImGuiDrawRenderOperator::NAME;
        }

        [[nodiscard]] ParamsCollection defaults() const override {
            return ParamsCollection::empty();
        }

        [[nodiscard]] RenderOperator *create(const RenderOperatorFactory::Context &context) const override;

    private:
        VulkanBackend *_vulkanBackend;
        DeviceContext *_deviceContext;
        PresentContext *_presentContext;
    };
}

#endif // PENROSE_BUILTIN_RENDERING_IMGUI_DRAW_RENDER_OPERATOR_HPP
