#include "RenderContext.hpp"

#include "src/Events/EventQueue.hpp"
#include "src/Resources/ResourceSet.hpp"

namespace Penrose {

    void RenderContext::registerRenderOperator(const std::string &name,
                                               RenderOperatorDefaults defaultsFunc,
                                               RenderOperatorCreate factoryFunc) {
        this->_operatorDefaultsFuncs.insert_or_assign(name, defaultsFunc);
        this->_operatorCreateFuncs.insert_or_assign(name, factoryFunc);
    }

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

    std::optional<RenderOperatorParams> RenderContext::tryGetRenderOperatorDefaults(const std::string &name) const {
        auto it = this->_operatorDefaultsFuncs.find(name);

        if (it == this->_operatorDefaultsFuncs.end()) {
            return std::nullopt;
        }

        return it->second();
    }

    std::optional<std::unique_ptr<RenderOperator>> RenderContext::tryCreateRenderOperator(
            const std::string &name,
            const RenderOperatorCreateContext &createContext) const {
        auto it = this->_operatorCreateFuncs.find(name);

        if (it == this->_operatorCreateFuncs.end()) {
            return std::nullopt;
        }

        return it->second(createContext);
    }
}
