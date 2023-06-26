#ifndef PENROSE_RENDERING_RENDER_CONTEXT_HPP
#define PENROSE_RENDERING_RENDER_CONTEXT_HPP

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <thread>

#include "src/Rendering/RenderList.hpp"
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

        std::mutex _contextMutex;
        std::optional<RenderGraph> _graph;
        std::map<std::string, RenderList> _lists;
        std::map<std::string, RenderOperatorDefaults> _operatorDefaultsFuncs;
        std::map<std::string, RenderOperatorCreate> _operatorCreateFuncs;

        void registerRenderOperator(const std::string &name,
                                    RenderOperatorDefaults defaultsFunc,
                                    RenderOperatorCreate createFunc);

    public:
        explicit RenderContext(ResourceSet *resources);
        ~RenderContext() override = default;

        [[nodiscard]] std::lock_guard<std::mutex> acquireContextLock() {
            return std::lock_guard<std::mutex>(this->_contextMutex);
        }

        void setRenderGraph(const std::optional<RenderGraph> &graph);

        [[nodiscard]] const std::optional<RenderGraph> &getRenderGraph() { return this->_graph; }

        void clearRenderLists();
        void setRenderList(const std::string &name, const RenderList &list);
        [[nodiscard]] std::optional<RenderList> tryGetRenderList(const std::string &name) const;

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
