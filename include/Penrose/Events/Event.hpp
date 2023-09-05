#ifndef PENROSE_EVENTS_EVENT_HPP
#define PENROSE_EVENTS_EVENT_HPP

#include <functional>
#include <optional>
#include <string>
#include <variant>

#include <Penrose/Common/Size.hpp>
#include <Penrose/ECS/Entity.hpp>

namespace Penrose {

    enum class EventType {
        EngineDestroyRequested,

        EntityCreated,
        EntityDestroyed,
        ComponentCreated,
        ComponentDestroyed,

        SurfaceResized
    };

    struct EntityEventValue {
        Entity entity;
    };

    struct ComponentEventValue {
        Entity entity;
        std::string componentName;
    };

    using EventValue = std::variant<
            Size,
            EntityEventValue,
            ComponentEventValue
    >;

    struct Event {
        EventType type;
        std::optional<EventValue> value;

        [[nodiscard]] std::optional<const ComponentEventValue *> tryGetComponentEvent() const;
    };

    [[nodiscard]] Event makeEvent(const EventType &type);
    [[nodiscard]] Event makeEvent(const EventType &type, const EventValue &value);

    using EventHandler = std::function<void(const Event &)>;
    using EventHandlerIndex = std::uint32_t;
}

#endif // PENROSE_EVENTS_EVENT_HPP
