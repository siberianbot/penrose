#ifndef PENROSE_RENDERING_RENDER_GRAPH_CONTEXT_HPP
#define PENROSE_RENDERING_RENDER_GRAPH_CONTEXT_HPP

#include <optional>

#include <Penrose/Rendering/RenderGraphInfo.hpp>
#include <Penrose/Rendering/RenderGraphHook.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;

    class RenderGraphContext : public Resource {
    public:
        explicit RenderGraphContext(ResourceSet *resources);
        ~RenderGraphContext() override = default;

        void setRenderGraph(const std::optional<RenderGraphInfo> &graphInfo);

        [[nodiscard]] const std::optional<RenderGraphInfo> &getRenderGraph() { return this->_graphInfo; }

    private:
        LazyCollection<RenderGraphHook> _hooks;

        std::optional<RenderGraphInfo> _graphInfo;
    };
}

#endif // PENROSE_RENDERING_RENDER_GRAPH_CONTEXT_HPP