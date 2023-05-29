#ifndef PENROSE_RENDERING_RENDER_CONTEXT_HPP
#define PENROSE_RENDERING_RENDER_CONTEXT_HPP

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#include "src/Rendering/RenderGraph.hpp"
#include "src/Rendering/Operators/RenderOperator.hpp"
#include "src/Resources/Resource.hpp"

namespace Penrose {

    class EventQueue;

    template<typename T>
    concept IsRenderOperatorProducer = std::is_base_of<RenderOperatorProducer, T>::value &&
                                       std::is_default_constructible<T>::value;

    class RenderContext : public Resource {
    private:
        EventQueue *_eventQueue;

        std::optional<RenderGraph> _graph;
        std::map<std::string, std::unique_ptr<RenderOperatorProducer>> _operatorProducers;

    public:
        explicit RenderContext(ResourceSet *resources);
        ~RenderContext() override = default;

        void setRenderGraph(const std::optional<RenderGraph> &graph);

        [[nodiscard]] const std::optional<RenderGraph> &getRenderGraph() { return this->_graph; }

        void addRenderOperatorProducer(const std::string_view &name,
                                       std::unique_ptr<RenderOperatorProducer> &&instance);

        template<IsRenderOperatorProducer T>
        void addRenderOperatorProducer(const std::string_view &name);

        [[nodiscard]] std::optional<RenderOperatorProducer *> tryGetRenderOperatorProducer(
                const std::string &name) const;
    };
}

#include "RenderContext.inl"

#endif // PENROSE_RENDERING_RENDER_CONTEXT_HPP
