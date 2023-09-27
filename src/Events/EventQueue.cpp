#include <Penrose/Events/EventQueue.hpp>

#include <utility>

namespace Penrose {

    void EventQueue::destroy() {
        for (auto &eventQueue: this->_eventQueues) {
            eventQueue.clear();
        }
    }

    void EventQueue::update(float) {
        auto &previousEventQueue = this->_eventQueues.at(this->_currentEventQueueIdx);
        this->_currentEventQueueIdx = (this->_currentEventQueueIdx + 1) % EVENT_QUEUE_SIZE;

        for (const auto &event: previousEventQueue) {
            for (const auto &[_, entry]: this->_handlers) {
                if ((entry.mask & event->getType()) == 0) {
                    continue;
                }

                entry.handler(event.get());
            }
        }

        previousEventQueue.clear();
    }

    EventQueue::HandlerIdx EventQueue::addHandler(EventTypeMask mask, EventQueue::Handler &&handler) {
        auto idx = this->_lastHandlerIdx++;
        auto entry = HandlerEntry{
                .mask = mask,
                .handler = handler
        };

        this->_handlers.insert_or_assign(idx, entry);

        return idx;
    }

    void EventQueue::removeHandler(EventQueue::HandlerIdx idx) {
        this->_handlers.erase(idx);
    }

    void EventQueue::pushEvent(std::unique_ptr<EventBase> &&event) {
        this->_eventQueues.at(this->_currentEventQueueIdx).push_back(std::forward<decltype(event)>(event));
    }
}