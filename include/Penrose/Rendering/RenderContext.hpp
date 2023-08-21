#ifndef PENROSE_RENDERING_RENDER_CONTEXT_HPP
#define PENROSE_RENDERING_RENDER_CONTEXT_HPP

#include <optional>

#include <Penrose/Rendering/RenderGraphInfo.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;
    class EventQueue;

    class RenderContext : public Resource {
    public:
        explicit RenderContext(ResourceSet *resources);
        ~RenderContext() override = default;

        void setRenderGraph(const std::optional<RenderGraphInfo> &graph);

        [[nodiscard]] const std::optional<RenderGraphInfo> &getRenderGraph() { return this->_graph; }

    private:
        EventQueue *_eventQueue;

        std::optional<RenderGraphInfo> _graph;
    };
}

#endif // PENROSE_RENDERING_RENDER_CONTEXT_HPP
