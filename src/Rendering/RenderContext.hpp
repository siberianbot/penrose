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

    class ResourceSet;
    class EventQueue;

    template<typename T>
    concept IsRenderOperator = std::is_base_of<RenderOperator, T>::value;

    template<typename T>
    concept IsDefaultConstructableRenderOperator = IsRenderOperator<T> && std::is_default_constructible<T>::value;

    template<typename T>
    concept IsConstructableWithResourceSetRenderOperator = IsRenderOperator<T> && requires(ResourceSet *resources) {
        T(resources);
    };

    class RenderContext : public Resource {
    private:
        ResourceSet *_resources;
        EventQueue *_eventQueue;

        std::optional<RenderGraph> _graph;
        std::map<std::string, std::unique_ptr<RenderOperator>> _operators;

    public:
        explicit RenderContext(ResourceSet *resources);
        ~RenderContext() override = default;

        void setRenderGraph(const std::optional<RenderGraph> &graph);

        [[nodiscard]] const std::optional<RenderGraph> &getRenderGraph() { return this->_graph; }

        void addRenderOperator(const std::string_view &name, std::unique_ptr<RenderOperator> &&instance);

        template<IsDefaultConstructableRenderOperator T>
        void addRenderOperator(const std::string_view &name);

        template<IsConstructableWithResourceSetRenderOperator T>
        void addRenderOperator(const std::string_view &name);

        [[nodiscard]] std::optional<RenderOperator *> tryGetRenderOperator(const std::string &name) const;
    };
}

#include "RenderContext.inl"

#endif // PENROSE_RENDERING_RENDER_CONTEXT_HPP
