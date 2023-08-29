#ifndef PENROSE_BUILTIN_RENDERING_IMGUI_DRAW_RENDER_OPERATOR_HPP
#define PENROSE_BUILTIN_RENDERING_IMGUI_DRAW_RENDER_OPERATOR_HPP

#include <cstdint>
#include <optional>
#include <string_view>

#include <Penrose/Rendering/RenderOperator.hpp>
#include <Penrose/Resources/Initializable.hpp>

namespace Penrose {

    class ResourceSet;
    class VulkanBackend;
    class DeviceContext;
    class PresentContext;
    class RenderSubgraph;

    // TODO: this operator should be abstract
    class ImGuiDrawRenderOperator : public RenderOperator, public Initializable {
    public:
        constexpr static const std::string_view NAME = "ImGuiDraw";

        explicit ImGuiDrawRenderOperator(ResourceSet *resources);
        ~ImGuiDrawRenderOperator() override = default;

        void init() override { /* nothing to do */ }

        void destroy() override;

        [[nodiscard]] std::string getName() const override { return std::string(NAME); }

        [[nodiscard]] ParamsCollection getDefaults() const override { return ParamsCollection::empty(); }

        void execute(CommandRecording *commandRecording, const RenderOperator::Context &context) override;

    private:
        struct State {
            RenderSubgraph *subgraph;
            std::uint32_t subgraphPassIdx;
        };

        VulkanBackend *_vulkanBackend;
        DeviceContext *_deviceContext;
        PresentContext *_presentContext;

        std::optional<State> _state;

        void initFor(const RenderOperator::Context &context);
    };
}

#endif // PENROSE_BUILTIN_RENDERING_IMGUI_DRAW_RENDER_OPERATOR_HPP
