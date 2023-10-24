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

    // TODO: pass component info
    template<typename Self>
    class ComponentEvent : public EntityEvent<Self> {
    public:
        ~ComponentEvent() override = default;

        [[nodiscard]] const std::string &getComponentName() const { return this->_componentName; }

    protected:
        explicit ComponentEvent(Entity entity, std::string &&componentName)
                : EntityEvent<Self>(entity),
                  _componentName(componentName) {
            //
        }

    private:
        std::string _componentName;
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
        explicit ComponentCreatedEvent(Entity entity, std::shared_ptr<Component> &&component)
                : ComponentEvent<ComponentCreatedEvent>(entity, component->getName()),
                  _component(component) {
            //
        }

        ~ComponentCreatedEvent() override = default;

        [[nodiscard]] const std::shared_ptr<Component> &getComponent() const { return this->_component; }

    private:
        std::shared_ptr<Component> _component;
    };

    class ComponentDestroyedEvent : public ComponentEvent<ComponentDestroyedEvent> {
    public:
        explicit ComponentDestroyedEvent(Entity entity, std::string &&componentName)
                : ComponentEvent<ComponentDestroyedEvent>(entity,
                                                          std::forward<decltype(componentName)>(componentName)) {
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
