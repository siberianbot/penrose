#include <Penrose/Rendering/RenderGraphContext.hpp>

#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    RenderGraphContext::RenderGraphContext(ResourceSet *resources)
            : _hooks(resources->getAllLazy<RenderGraphHook>()) {
        //
    }

    void RenderGraphContext::setRenderGraph(const std::optional<RenderGraphInfo> &graphInfo) {
        this->_graphInfo = graphInfo;

        for (const auto &hook: this->_hooks) {
            hook->onRenderGraphModified(this->_graphInfo);
        }
    }
}
