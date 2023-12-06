#include <Penrose/Builtin/ImGui/ImGui.hpp>

#include "src/Builtin/ImGui/ImGuiBackend.hpp"
#include "src/Builtin/ImGui/Rendering/VkImGuiRenderOperator.hpp"
#include "src/Builtin/ImGui/UI/ImGuiUIContextVisitor.hpp"

namespace Penrose {

    ResourceSet &addImGui(ResourceSet &resources) {

        resources.add<ImGuiBackend>().group(ResourceGroup::Backend)
            .implements<Initializable>()
            .implements<SurfaceHook>()
            .implements<InputHook>()
            .done();

        resources.add<ImGuiUIContextVisitor>().group(ResourceGroup::UI).done();

        resources.add<VkImGuiRenderOperator>().group(ResourceGroup::RenderOperator)
            .implements<Initializable>()
            .implements<RenderOperator>()
            .done();

        return resources;
    }
}
