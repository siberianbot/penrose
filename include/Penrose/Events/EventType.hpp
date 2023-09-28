#ifndef PENROSE_EVENTS_EVENT_TYPE_HPP
#define PENROSE_EVENTS_EVENT_TYPE_HPP

#include <cstdint>

namespace Penrose {

    using EventTypeMask = std::uint32_t;

    enum class EventType : EventTypeMask {

        EngineEvent = 1,
        ECSEvent = EngineEvent << 1,
        SurfaceEvent = ECSEvent << 1,
        InputEvent = SurfaceEvent << 1,

        All = EngineEvent | ECSEvent | SurfaceEvent | InputEvent
    };

    constexpr EventTypeMask operator&(const EventTypeMask &a, const EventType &b) {
        return a & static_cast<EventTypeMask>(b);
    }

    constexpr EventTypeMask operator|(const EventType &a, const EventType &b) {
        return static_cast<EventTypeMask>(a) | static_cast<EventTypeMask>(b);
    }

    constexpr EventTypeMask operator&(const EventType &a, const EventType &b) {
        return static_cast<EventTypeMask>(a) & static_cast<EventTypeMask>(b);
    }
}

#endif // PENROSE_EVENTS_EVENT_TYPE_HPP
