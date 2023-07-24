#ifndef PENROSE_RENDERING_RENDER_CONTEXT_HPP
#define PENROSE_RENDERING_RENDER_CONTEXT_HPP

#include <map>
#include <optional>
#include <string>
#include <thread>

#include <Penrose/Rendering/RenderGraph.hpp>
#include <Penrose/Rendering/RenderList.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;
    class EventQueue;

    class RenderContext : public Resource {
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

    private:
        EventQueue *_eventQueue;

        std::mutex _contextMutex;
        std::optional<RenderGraph> _graph;
        std::map<std::string, RenderList> _lists;
    };
}

#endif // PENROSE_RENDERING_RENDER_CONTEXT_HPP
