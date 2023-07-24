#include <Penrose/Rendering/RenderContext.hpp>

#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    RenderContext::RenderContext(ResourceSet *resources)
            : _eventQueue(resources->get<EventQueue>()) {
        //
    }

    void RenderContext::setRenderGraph(const std::optional<RenderGraph> &graph) {
        this->_graph = graph;
        this->_eventQueue->push(makeEvent(EventType::RenderGraphModified));
    }

    void RenderContext::clearRenderLists() {
        this->_lists.clear();
        this->_eventQueue->push(makeEvent(EventType::RenderListsModified));
    }

    void RenderContext::setRenderList(const std::string &name, const RenderList &list) {
        this->_lists[name] = list;
        this->_eventQueue->push(makeEvent(EventType::RenderListsModified));
    }

    std::optional<RenderList> RenderContext::tryGetRenderList(const std::string &name) const {
        auto it = this->_lists.find(name);

        if (it == this->_lists.end()) {
            return std::nullopt;
        }

        auto result = it->second;

        return result;
    }
}
