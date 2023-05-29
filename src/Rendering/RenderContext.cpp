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

    void RenderContext::addRenderOperatorProducer(const std::string_view &name,
                                                  std::unique_ptr<RenderOperatorProducer> &&instance) {
        this->_operatorProducers.emplace(name, std::move(instance));
        this->_eventQueue->push(makeEvent(EventType::RenderContextModified));
    }

    std::optional<RenderOperatorProducer *> RenderContext::tryGetRenderOperatorProducer(
            const std::string &name) const {
        auto it = this->_operatorProducers.find(name);

        if (it != this->_operatorProducers.end()) {
            return it->second.get();
        }

        return std::nullopt;
    }
}
