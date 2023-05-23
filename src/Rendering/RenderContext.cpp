#include "RenderContext.hpp"

#include "src/Events/EventQueue.hpp"
#include "src/Resources/ResourceSet.hpp"

namespace Penrose {

    RenderContext::RenderContext(ResourceSet *resources)
            : _eventQueue(resources->get<EventQueue>()) {
        //
    }

    void RenderContext::setRenderGraph(const std::optional<RenderGraph> &graph) {
        this->_graph = graph;
        this->_eventQueue->push(makeEvent(EventType::RenderContextModified));
    }
}