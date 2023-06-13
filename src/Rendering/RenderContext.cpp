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

    void RenderContext::addRenderOperatorFactory(const std::string_view &name, RenderOperatorFactory factory) {
        this->_operatorFactories.emplace(name, factory);
        this->_eventQueue->push(makeEvent(EventType::RenderContextModified));
    }

    std::optional<RenderOperatorFactory> RenderContext::tryGetRenderOperatorFactory(const std::string &name) const {
        auto it = this->_operatorFactories.find(name);

        if (it != this->_operatorFactories.end()) {
            return it->second;
        }

        return std::nullopt;
    }
}
