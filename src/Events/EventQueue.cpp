#include "EventQueue.hpp"

namespace Penrose {

    void EventQueue::push(Event event) {
        this->_events.front().push_back(event);
    }

    void EventQueue::process() {
        this->_events.swap();

        for (const auto &event: this->_events.back()) {
            for (const auto &[_, handler]: this->_handlers) {
                handler(event);
            }
        }

        this->_events.back().clear();
    }

    EventHandlerIndex EventQueue::addHandler(const EventHandler &handler) {
        auto idx = this->_nextHandlerIdx++;
        this->_handlers.emplace(idx, handler);

        return idx;
    }

    void EventQueue::removeHandler(const EventHandlerIndex &idx) {
        auto it = this->_handlers.find(idx);

        if (it == this->_handlers.end()) {
            return;
        }

        this->_handlers.erase(it);
    }
}