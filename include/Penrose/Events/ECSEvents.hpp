#ifndef PENROSE_EVENTS_ECS_EVENTS_HPP
#define PENROSE_EVENTS_ECS_EVENTS_HPP

#include <memory>
#include <string>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Events/EventQueue.hpp>

namespace Penrose {

    template<typename Self>
    class EntityEvent : public Event<Self> {
    public:
        ~EntityEvent() override = default;

        [[nodiscard]] const Entity &getEntity() const { return this->_entity; }

    protected:
        explicit EntityEvent(Entity entity)
                : _entity(entity) {
            //
        }

    private:
        Entity _entity;
    };

    template<typename Self>
    class ComponentEvent : public EntityEvent<Self> {
    public:
        ~ComponentEvent() override = default;

        [[nodiscard]] const ComponentInfo &getComponentType() const { return this->_componentType; }

    protected:
        explicit ComponentEvent(Entity entity, ComponentInfo &&componentType)
                : EntityEvent<Self>(entity),
                  _componentType(componentType) {
            //
        }

    private:
        ComponentInfo _componentType;
    };

    class EntityCreatedEvent : public EntityEvent<EntityCreatedEvent> {
    public:
        explicit EntityCreatedEvent(Entity entity)
                : EntityEvent<EntityCreatedEvent>(entity) {
            //
        }

        ~EntityCreatedEvent() override = default;
    };

    class EntityDestroyedEvent : public EntityEvent<EntityDestroyedEvent> {
    public:
        explicit EntityDestroyedEvent(Entity entity)
                : EntityEvent<EntityDestroyedEvent>(entity) {
            //
        }

        ~EntityDestroyedEvent() override = default;
    };

    class ComponentCreatedEvent : public ComponentEvent<ComponentCreatedEvent> {
    public:
        explicit ComponentCreatedEvent(Entity entity, std::shared_ptr<ComponentBase> &&component)
                : ComponentEvent<ComponentCreatedEvent>(entity, component->getType()),
                  _component(component) {
            //
        }

        ~ComponentCreatedEvent() override = default;

        [[nodiscard]] const std::shared_ptr<ComponentBase> &getComponent() const { return this->_component; }

    private:
        std::shared_ptr<ComponentBase> _component;
    };

    class ComponentDestroyedEvent : public ComponentEvent<ComponentDestroyedEvent> {
    public:
        explicit ComponentDestroyedEvent(Entity entity, ComponentInfo &&componentType)
                : ComponentEvent<ComponentDestroyedEvent>(entity,
                                                          std::forward<decltype(componentType)>(componentType)) {
            //
        }

        ~ComponentDestroyedEvent() override = default;
    };

    using ECSEventQueue = EventQueue<
            EntityCreatedEvent,
            EntityDestroyedEvent,
            ComponentCreatedEvent,
            ComponentDestroyedEvent>;
}

#endif // PENROSE_EVENTS_ECS_EVENTS_HPP
