#ifndef PENROSE_BUILTIN_IMGUI_RENDERING_IMGUI_RENDER_OPERATOR_HPP
#define PENROSE_BUILTIN_IMGUI_RENDERING_IMGUI_RENDER_OPERATOR_HPP

#include <string>
#include <string_view>

#include <Penrose/Rendering/RenderOperator.hpp>

namespace Penrose {

    class ImGuiRenderOperator : public RenderOperator {
    public:
        constexpr static const std::string_view PARAM_UI_NAME = "UIName";

        ~ImGuiRenderOperator() override = default;

        [[nodiscard]] constexpr static std::string name() { return "ImGui"; }

        [[nodiscard]] std::string getName() const override { return name(); }

        [[nodiscard]] ParamsCollection getDefaults() const override {
            ParamsCollection params;
            params.setString(PARAM_UI_NAME, "Default");

            return params;
        }
    };
}

#endif // PENROSE_BUILTIN_IMGUI_RENDERING_IMGUI_RENDER_OPERATOR_HPP
