#ifndef PENROSE_EVENTS_EVENT_QUEUE_HPP
#define PENROSE_EVENTS_EVENT_QUEUE_HPP

#include <unordered_map>

#include <Penrose/Common/DoubleVector.hpp>
#include <Penrose/Events/Event.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class EventQueue : public Resource {
    private:
        DoubleVector<Event> _events;

        std::uint32_t _nextHandlerIdx;
        std::unordered_map<EventHandlerIndex, EventHandler> _handlers;

    public:
        ~EventQueue() override = default;

        void push(Event event);
        void process();

        EventHandlerIndex addHandler(const EventHandler &handler);
        void removeHandler(const EventHandlerIndex &idx);
    };
}

#endif // PENROSE_EVENTS_EVENT_QUEUE_HPP
