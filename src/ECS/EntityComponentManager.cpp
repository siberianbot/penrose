#include "EntityComponentManager.hpp"

#include <fmt/core.h>

#include "src/Common/EngineError.hpp"
#include "src/Events/EventQueue.hpp"
#include "src/Resources/ResourceSet.hpp"

namespace Penrose {

    EntityComponentManager::EntityComponentManager(ResourceSet *resources)
            : _eventQueue(resources->get<EventQueue>()) {
        //
    }

    void EntityComponentManager::destroy() {
        this->_components.clear();
        this->_entities.clear();
    }

    ComponentType EntityComponentManager::createComponentType(std::string_view &&name, ComponentFactory &&factory) {
        auto componentType = this->_nextComponentType++;

        this->_componentTypes[componentType] = ComponentTypeDescription{
                .name = std::string(name),
                .factory = factory
        };

        return componentType;
    }

    Entity EntityComponentManager::createEntity() {
        auto entity = this->_nextEntity++;
        this->_entities.emplace(entity, EntityDescription{});

        auto event = makeEvent(EventType::EntityCreated, EntityEventValue{
                .entity = entity
        });
        this->_eventQueue->push(event);

        return entity;
    }

    void EntityComponentManager::destroyEntity(const Entity &entity) {
        auto it = this->_entities.find(entity);

        if (it == this->_entities.end()) {
            // TODO: notify
            return;
        }

        auto componentTypes = it->second.componentTypes;
        for (const auto &componentType: componentTypes) {
            auto componentId = makeComponentId(entity, componentType);
            this->_components.erase(componentId);
        }

        this->_entities.erase(it);

        auto event = makeEvent(EventType::EntityDestroyed, EntityEventValue{
                .entity = entity
        });
        this->_eventQueue->push(event);
    }

    ComponentId EntityComponentManager::createComponent(const Entity &entity, const ComponentType &componentType) {
        auto componentTypeIt = this->_componentTypes.find(componentType);
        if (componentTypeIt == this->_componentTypes.end()) {
            throw EngineError(fmt::format("Unknown component type {}", componentType));
        }

        auto entityIt = this->_entities.find(entity);
        if (entityIt == this->_entities.end()) {
            throw EngineError(fmt::format("Entity {} not found", entity));
        }

        if (entityIt->second.componentTypes.contains(componentType)) {
            throw EngineError(fmt::format("Entity {} already have component {}", entity, componentTypeIt->second.name));
        }

        entityIt->second.componentTypes.emplace(componentType);

        auto componentId = makeComponentId(entity, componentType);
        this->_components[componentId] = componentTypeIt->second.factory();

        auto event = makeEvent(EventType::ComponentCreated, ComponentEventValue{
                .entity = entity,
                .componentType = componentType,
                .componentId = componentId
        });
        this->_eventQueue->push(event);

        return componentId;
    }

    void EntityComponentManager::destroyComponent(const ComponentId &componentId) {
        auto [entity, componentType] = deconstructComponentId(componentId);

        auto entityIt = this->_entities.find(entity);
        if (entityIt == this->_entities.end()) {
            // TODO: notify
            return;
        }

        entityIt->second.componentTypes.erase(componentType);
        this->_components.erase(componentId);

        auto event = makeEvent(EventType::ComponentDestroyed, ComponentEventValue{
                .entity = entity,
                .componentType = componentType,
                .componentId = componentId
        });
        this->_eventQueue->push(event);
    }

    std::optional<std::weak_ptr<Component>> EntityComponentManager::tryGetComponent(const Entity &entity,
                                                                                    const ComponentType &componentType) {
        return this->tryGetComponent(makeComponentId(entity, componentType));
    }

    std::optional<std::weak_ptr<Component>> EntityComponentManager::tryGetComponent(const ComponentId &componentId) {
        auto componentIt = this->_components.find(componentId);

        if (componentIt == this->_components.end()) {
            return std::nullopt;
        }

        return componentIt->second;
    }
}
