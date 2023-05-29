#include "RenderContext.hpp"

#include "src/Events/EventQueue.hpp"
#include "src/Resources/ResourceSet.hpp"

namespace Penrose {

    RenderContext::RenderContext(ResourceSet *resources)
            : _resources(resources),
              _eventQueue(resources->get<EventQueue>()) {
        //
    }

    void RenderContext::setRenderGraph(const std::optional<RenderGraph> &graph) {
        this->_graph = graph;
        this->_eventQueue->push(makeEvent(EventType::RenderContextModified));
    }

    void RenderContext::addRenderOperator(const std::string_view &name, std::unique_ptr<RenderOperator> &&instance) {
        this->_operators.emplace(name, std::move(instance));
        this->_eventQueue->push(makeEvent(EventType::RenderContextModified));
    }

    std::optional<RenderOperator *> RenderContext::tryGetRenderOperator(const std::string &name) const {
        auto it = this->_operators.find(name);

        if (it != this->_operators.end()) {
            return it->second.get();
        }

        return std::nullopt;
    }
}
