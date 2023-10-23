#include <Penrose/Rendering/RenderGraphContext.hpp>

namespace Penrose {

    RenderGraphContext::RenderGraphContext(ResourceSet *resources)
            : _hooks(resources->get<RenderGraphHook>()) {
        //
    }

    void RenderGraphContext::setRenderGraph(const RenderGraphInfo &graphInfo) {
        this->_graphInfo = graphInfo;

        for (const auto &hook: this->_hooks) {
            hook->onRenderGraphModified(this->_graphInfo);
        }
    }
}
