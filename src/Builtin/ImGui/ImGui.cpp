#include <Penrose/Builtin/ImGui/ImGui.hpp>

#include "src/Builtin/ImGui/ImGuiBackend.hpp"
#include "src/Builtin/ImGui/Rendering/VkImGuiRenderer.hpp"
#include "src/Builtin/ImGui/UI/ImGuiUIContextVisitor.hpp"

namespace Penrose {

    ResourceSet &addImGui(ResourceSet &resources) {

        resources.add<ImGuiBackend>()
            .group(ResourceGroup::Backend)
            .implements<Initializable>()
            .implements<SurfaceHook>()
            .implements<InputHook>()
            .done();

        resources.add<ImGuiUIContextVisitor>().group(ResourceGroup::UI).done();

        resources.add<VkImGuiRenderer>()
            .group(ResourceGroup::Rendering)
            .implements<Renderer>()
            .implements<ImGuiRenderer>()
            .done();

        return resources;
    }
}
