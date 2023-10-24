#ifndef PENROSE_EVENTS_ENGINE_EVENTS_HPP
#define PENROSE_EVENTS_ENGINE_EVENTS_HPP

#include <Penrose/Events/EventQueue.hpp>

namespace Penrose {

    class EngineDestroyRequestedEvent : public Event<EngineDestroyRequestedEvent> {
    public:
        ~EngineDestroyRequestedEvent() override = default;
    };

    using EngineEventQueue = EventQueue<EngineDestroyRequestedEvent>;
}

#endif // PENROSE_EVENTS_ENGINE_EVENTS_HPP
