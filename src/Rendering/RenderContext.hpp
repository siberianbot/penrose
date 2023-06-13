#ifndef PENROSE_RENDERING_RENDER_CONTEXT_HPP
#define PENROSE_RENDERING_RENDER_CONTEXT_HPP

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#include "src/Rendering/RenderGraph.hpp"
#include "src/Rendering/Operators/RenderOperatorFactory.hpp"
#include "src/Resources/Resource.hpp"

namespace Penrose {

    class ResourceSet;
    class EventQueue;

    class RenderContext : public Resource {
    private:
        EventQueue *_eventQueue;

        std::optional<RenderGraph> _graph;
        std::map<std::string, RenderOperatorFactory> _operatorFactories;

    public:
        explicit RenderContext(ResourceSet *resources);
        ~RenderContext() override = default;

        void setRenderGraph(const std::optional<RenderGraph> &graph);

        [[nodiscard]] const std::optional<RenderGraph> &getRenderGraph() { return this->_graph; }

        void addRenderOperatorFactory(const std::string_view &name, RenderOperatorFactory factory);

        [[nodiscard]] std::optional<RenderOperatorFactory> tryGetRenderOperatorFactory(const std::string &name) const;
    };
}

#endif // PENROSE_RENDERING_RENDER_CONTEXT_HPP
