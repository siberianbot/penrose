#ifndef PENROSE_RENDERING_RENDER_CONTEXT_HPP
#define PENROSE_RENDERING_RENDER_CONTEXT_HPP

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>

#include "src/Rendering/RenderGraph.hpp"
#include "src/Rendering/Operators/RenderOperator.hpp"
#include "src/Resources/Resource.hpp"

namespace Penrose {

    class ResourceSet;
    class EventQueue;

    using RenderOperatorDefaults = std::function<RenderOperatorParams()>;
    using RenderOperatorCreate = std::function<std::unique_ptr<RenderOperator>(const RenderOperatorCreateContext &)>;

    class RenderContext : public Resource {
    private:
        EventQueue *_eventQueue;

        std::optional<RenderGraph> _graph;
        std::map<std::string, RenderOperatorDefaults> _operatorDefaultsFuncs;
        std::map<std::string, RenderOperatorCreate> _operatorCreateFuncs;

        void registerRenderOperator(const std::string &name,
                                    RenderOperatorDefaults defaultsFunc,
                                    RenderOperatorCreate createFunc);

    public:
        explicit RenderContext(ResourceSet *resources);
        ~RenderContext() override = default;

        void setRenderGraph(const std::optional<RenderGraph> &graph);

        [[nodiscard]] const std::optional<RenderGraph> &getRenderGraph() { return this->_graph; }

        template<IsRenderOperator T>
        void registerRenderOperator();

        template<IsRenderOperatorWithDefaults T>
        void registerRenderOperator();

        [[nodiscard]] std::optional<RenderOperatorParams> tryGetRenderOperatorDefaults(const std::string &name) const;

        [[nodiscard]] std::optional<std::unique_ptr<RenderOperator>> tryCreateRenderOperator(
                const std::string &name,
                const RenderOperatorCreateContext &createContext) const;
    };
}

#include "RenderContext.inl"

#endif // PENROSE_RENDERING_RENDER_CONTEXT_HPP
