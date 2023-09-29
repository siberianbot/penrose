#include <Penrose/Builtin/ImGui.hpp>

#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/ImGui/ImGuiBackend.hpp"

namespace Penrose {

    ResourceSet &addImGui(ResourceSet &resources) {

        resources.add<ImGuiBackend, SurfaceHook, InputHook>(resources.getBeginIterator());

        return resources;
    }
}
