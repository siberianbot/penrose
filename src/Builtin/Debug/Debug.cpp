#include <Penrose/Builtin/Debug/Debug.hpp>

#include <Penrose/Resources/ResourceSet.hpp>

#include <Penrose/Builtin/Debug/UI/UIContext.hpp>

#include "src/Builtin/Debug/Rendering/VkImGuiDebugUIDrawRenderOperator.hpp"

namespace Penrose {

    ResourceSet &addDebug(ResourceSet &resources) {

        resources.add<UIContext>();
        resources.add<VkImGuiDebugUIDrawRenderOperator, RenderOperator>();

        return resources;
    }
}
