#ifndef PENROSE_EVENTS_EVENT_HPP
#define PENROSE_EVENTS_EVENT_HPP

#include <optional>
#include <variant>

#include "src/Common/Types.hpp"

namespace Penrose {

    enum class EventType {
        EngineDestroyRequested,
        SurfaceResized,
    };

    using EventValue = std::variant<SurfaceSize>;

    struct Event {
        EventType type;
        std::optional<EventValue> value;
    };

    [[nodiscard]] Event makeEvent(const EventType &type);
    [[nodiscard]] Event makeEvent(const EventType &type, const EventValue &value);
}

#endif // PENROSE_EVENTS_EVENT_HPP
