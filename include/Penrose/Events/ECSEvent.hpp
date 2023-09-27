#ifndef PENROSE_EVENTS_ECS_EVENT_HPP
#define PENROSE_EVENTS_ECS_EVENT_HPP

#include <memory>
#include <string>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Events/Event.hpp>

namespace Penrose {

    enum class ECSEventType {
        EntityCreated,
        EntityDestroyed,
        ComponentCreated,
        ComponentDestroyed
    };

    struct ECSEventArgs {
        ECSEventType type;
        Entity entity;
        std::string componentName;
        std::shared_ptr<Component> component;
    };

    using ECSEvent = Event<EventType::ECSEvent, ECSEventArgs>;
}

#endif // PENROSE_EVENTS_ECS_EVENT_HPP
