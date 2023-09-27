#ifndef PENROSE_EVENTS_ENGINE_EVENT_HPP
#define PENROSE_EVENTS_ENGINE_EVENT_HPP

#include <Penrose/Events/Event.hpp>

namespace Penrose {

    enum class EngineEventType {
        DestroyRequested
    };

    struct EngineEventArgs {
        EngineEventType type;
    };

    using EngineEvent = Event<EventType::EngineEvent, EngineEventArgs>;
}

#endif // PENROSE_EVENTS_ENGINE_EVENT_HPP
