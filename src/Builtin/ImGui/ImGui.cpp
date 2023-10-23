#include <Penrose/Builtin/ImGui.hpp>

#include <Penrose/Rendering/SurfaceManager.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/ImGui/ImGuiBackend.hpp"

namespace Penrose {

    ResourceSet &addImGui(ResourceSet &resources) {

        resources.add<ImGuiBackend>()
                .implements<Initializable>()
                .implements<SurfaceHook>()
                .implements<InputHook>()
                .before<SurfaceManager>()
                .done();

        return resources;
    }
}
