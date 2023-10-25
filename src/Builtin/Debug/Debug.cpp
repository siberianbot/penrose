#include <Penrose/Builtin/Debug/Debug.hpp>

#include <Penrose/Builtin/Debug/UI/UIContext.hpp>

#include "src/Builtin/Debug/Rendering/VkImGuiDebugUIDrawRenderOperator.hpp"
#include "src/Builtin/Debug/UI/UIDrawVisitor.hpp"

namespace Penrose {

    ResourceSet &addDebug(ResourceSet &resources) {

        resources.add<UIContext, ResourceGroup::Rendering>().done();
        resources.add<UIDrawVisitor, ResourceGroup::Rendering>().done();

        resources.add<VkImGuiDebugUIDrawRenderOperator, ResourceGroup::RenderOperator>()
                .implements<Initializable>()
                .implements<RenderOperator>()
                .done();

        return resources;
    }
}
