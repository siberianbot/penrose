#include <Penrose/Builtin/Debug/Debug.hpp>

#include <Penrose/Resources/ResourceSet.hpp>

#include <Penrose/Builtin/Debug/UI/UIContext.hpp>

#include "src/Builtin/Debug/Rendering/VkImGuiDebugUIDrawRenderOperator.hpp"
#include "src/Builtin/Debug/UI/UIDrawVisitor.hpp"

namespace Penrose {

    ResourceSet &addDebug(ResourceSet &resources) {

        resources.add<UIContext>();
        resources.add<UIDrawVisitor>();
        resources.add<VkImGuiDebugUIDrawRenderOperator, RenderOperator>();

        return resources;
    }
}
