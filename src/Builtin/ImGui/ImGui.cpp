#include <Penrose/Builtin/ImGui.hpp>

#include "src/Builtin/ImGui/ImGuiBackend.hpp"

namespace Penrose {

    ResourceSet &addImGui(ResourceSet &resources) {

        resources.add<ImGuiBackend, ResourceGroup::Backend>()
                .implements<Initializable>()
                .implements<SurfaceHook>()
                .implements<InputHook>()
                .done();

        return resources;
    }
}
