#ifndef PENROSE_EVENTS_EVENT_HPP
#define PENROSE_EVENTS_EVENT_HPP

#include <optional>
#include <variant>

#include <string>

namespace Penrose {

    enum class EventType {
        EngineDestroyRequested
    };

    struct Event {
        EventType type;
        // TODO: value
    };
}

#endif // PENROSE_EVENTS_EVENT_HPP
