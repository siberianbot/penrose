#ifndef PENROSE_EVENTS_EVENT_HPP
#define PENROSE_EVENTS_EVENT_HPP

#include <functional>
#include <optional>
#include <variant>

#include "src/Common/Types.hpp"

namespace Penrose {

    enum class EventType {
        EngineDestroyRequested,
        SurfaceResized,
    };

    using EventValue = std::variant<Size>;

    struct Event {
        EventType type;
        std::optional<EventValue> value;
    };

    [[nodiscard]] Event makeEvent(const EventType &type);
    [[nodiscard]] Event makeEvent(const EventType &type, const EventValue &value);

    using EventHandler = std::function<void(const Event &)>;
    using EventHandlerIndex = std::uint32_t;
}

#endif // PENROSE_EVENTS_EVENT_HPP
