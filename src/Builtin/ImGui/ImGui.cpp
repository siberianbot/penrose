#include <Penrose/Builtin/ImGui/ImGui.hpp>

#include "src/Builtin/ImGui/ImGuiBackend.hpp"
#include "src/Builtin/ImGui/Rendering/VkImGuiRenderOperator.hpp"
#include "src/Builtin/ImGui/UI/ImGuiUIInstanceVisitor.hpp"

namespace Penrose {

    ResourceSet &addImGui(ResourceSet &resources) {

        resources.add<ImGuiBackend, ResourceGroup::Backend>()
                .implements<Initializable>()
                .implements<SurfaceHook>()
                .implements<InputHook>()
                .done();

        resources.add<ImGuiUIInstanceVisitor, ResourceGroup::UI>()
                .done();

        resources.add<VkImGuiRenderOperator, ResourceGroup::RenderOperator>()
                .implements<Initializable>()
                .implements<RenderOperator>()
                .done();

        return resources;
    }
}
