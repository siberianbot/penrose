#ifndef PENROSE_RENDERING_RENDER_GRAPH_CONTEXT_HPP
#define PENROSE_RENDERING_RENDER_GRAPH_CONTEXT_HPP

#include <optional>

#include <Penrose/Api.hpp>
#include <Penrose/Rendering/RenderGraphInfo.hpp>
#include <Penrose/Rendering/RenderGraphHook.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    class PENROSE_API RenderGraphContext : public Resource<RenderGraphContext> {
    public:
        explicit RenderGraphContext(const ResourceSet *resources);
        ~RenderGraphContext() override = default;

        void setRenderGraph(const RenderGraphInfo &graphInfo);

        [[nodiscard]] const RenderGraphInfo &getRenderGraph() { return this->_graphInfo; }

    private:
        ResourceProxy<RenderGraphHook> _hooks;

        RenderGraphInfo _graphInfo = RenderGraphInfo::makeDefault();
    };
}

#endif // PENROSE_RENDERING_RENDER_GRAPH_CONTEXT_HPP
