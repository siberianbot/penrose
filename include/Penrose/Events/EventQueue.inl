#ifndef PENROSE_EVENTS_EVENT_QUEUE_INL
#define PENROSE_EVENTS_EVENT_QUEUE_INL

#include <utility>

namespace Penrose {

    template<EventType Type, typename Data>
    EventQueue::HandlerIdx EventQueue::addHandler(std::function<void(const Event<Type, Data> *)> &&handler) {
        return this->addHandler(
                static_cast<EventTypeMask>(Type),
                [handler](const EventBase *eventBase) {
                    handler(dynamic_cast<const Event<Type, Data> *>(eventBase));
                }
        );
    }

    template<EventType Type, typename Data>
    void EventQueue::pushEvent(Data data) {
        auto event = std::make_unique<Event<Type, Data>>(std::forward<decltype(data)>(data));

        this->pushEvent(std::forward<decltype(event)>(event));
    }
}

#endif // PENROSE_EVENTS_EVENT_QUEUE_INL
