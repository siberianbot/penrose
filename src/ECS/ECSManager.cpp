#include <Penrose/ECS/ECSManager.hpp>

#include <algorithm>
#include <limits>

#include <fmt/core.h>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    constexpr inline int UINT32_DIGITS = std::numeric_limits<std::uint32_t>::digits;

    std::optional<ECSManager::ComponentTypeDescription *> ECSManager::tryGetComponentTypeDescription(
            const ComponentName &name) const {
        auto it = std::find_if(this->_componentTypes.begin(), this->_componentTypes.end(),
                               [&name](const ComponentTypeDescription &description) {
                                   return description.name == name;
                               });

        if (it == this->_componentTypes.end()) {
            return std::nullopt;
        }

        return const_cast<ECSManager::ComponentTypeDescription *>(&(*it));
    }

    ECSManager::ComponentTypeDescription *ECSManager::getComponentTypeDescription(const ComponentName &name) const {
        return orElseThrow(this->tryGetComponentTypeDescription(name),
                           EngineError(fmt::format("No component {} registered", name)));
    }

    ECSManager::ComponentTypeDescription *ECSManager::getComponentTypeDescription(
            const ECSManager::ComponentType &type) const {
        auto it = std::find_if(this->_componentTypes.begin(), this->_componentTypes.end(),
                               [&type](const ComponentTypeDescription &description) {
                                   return description.type == type;
                               });

        if (it == this->_componentTypes.end()) {
            throw EngineError(fmt::format("Unknown component type {}", type));
        }

        return const_cast<ECSManager::ComponentTypeDescription *>(&(*it));
    }

    ComponentId ECSManager::constructComponentId(const Entity &entity, const ECSManager::ComponentType &type) {
        ComponentId id = 0;

        id |= entity;
        id |= static_cast<std::uint64_t>(type) << UINT32_DIGITS;

        return id;
    }

    std::tuple<Entity, ECSManager::ComponentType> ECSManager::deconstructComponentId(const ComponentId &componentId) {
        auto entity = static_cast<std::uint32_t>(componentId) & static_cast<std::uint32_t>(-1);
        auto type = static_cast<std::uint32_t>(componentId >> UINT32_DIGITS) & static_cast<std::uint32_t>(-1);

        return std::make_tuple(entity, type);
    }

    ECSManager::ECSManager(ResourceSet *resources)
            : _resources(resources),
              _eventQueue(resources->get<EventQueue>()) {
        //
    }

    void ECSManager::init() {
        for (const auto &[_, system]: this->_systems) {
            system->init();
        }
    }

    void ECSManager::update() {
        for (const auto &[_, system]: this->_systems) {
            system->update();
        }
    }

    void ECSManager::destroy() {
        for (const auto &[_, system]: this->_systems) {
            system->destroy();
        }

        this->_components.clear();
        this->_entities.clear();
    }

    Entity ECSManager::createEntity() {
        auto entity = this->_nextEntity++;
        this->_entities.emplace(entity, EntityDescription{});

        auto event = makeEvent(EventType::EntityCreated, EntityEventValue{
                .entity = entity
        });
        this->_eventQueue->push(event);

        return entity;
    }

    void ECSManager::destroyEntity(const Entity &entity) {
        auto it = this->_entities.find(entity);

        if (it == this->_entities.end()) {
            // TODO: notify
            return;
        }

        auto componentTypes = it->second.componentTypes;
        for (const auto &componentType: componentTypes) {
            auto componentId = constructComponentId(entity, componentType);
            this->_components.erase(componentId);
        }

        this->_entities.erase(it);

        auto event = makeEvent(EventType::EntityDestroyed, EntityEventValue{
                .entity = entity
        });
        this->_eventQueue->push(event);
    }

    ComponentId ECSManager::createComponent(const Entity &entity, const ComponentName &name) {
        auto description = this->getComponentTypeDescription(name);

        auto entityIt = this->_entities.find(entity);
        if (entityIt == this->_entities.end()) {
            throw EngineError(fmt::format("Entity {} not found", entity));
        }

        if (entityIt->second.componentTypes.contains(description->type)) {
            throw EngineError(fmt::format("Entity {} already have component {}", entity, description->name));
        }

        entityIt->second.componentTypes.emplace(description->type);

        auto componentId = constructComponentId(entity, description->type);
        this->_components[componentId] = description->factory();

        auto event = makeEvent(EventType::ComponentCreated, ComponentEventValue{
                .entity = entity,
                .componentName = description->name,
                .componentId = componentId
        });
        this->_eventQueue->push(event);

        return componentId;
    }

    void ECSManager::destroyComponent(const ComponentId &componentId) {
        auto [entity, type] = deconstructComponentId(componentId);

        auto entityIt = this->_entities.find(entity);
        if (entityIt == this->_entities.end()) {
            // TODO: notify
            return;
        }

        entityIt->second.componentTypes.erase(type);
        this->_components.erase(componentId);

        auto description = this->getComponentTypeDescription(type);

        auto event = makeEvent(EventType::ComponentDestroyed, ComponentEventValue{
                .entity = entity,
                .componentName = description->name,
                .componentId = componentId
        });
        this->_eventQueue->push(event);
    }

    std::optional<std::shared_ptr<Component>> ECSManager::tryGetComponent(const Entity &entity,
                                                                          const ComponentName &name) const {
        auto description = this->getComponentTypeDescription(name);

        return this->tryGetComponent(constructComponentId(entity, description->type));
    }

    std::optional<std::shared_ptr<Component>> ECSManager::tryGetComponent(const ComponentId &componentId) const {
        auto componentIt = this->_components.find(componentId);

        if (componentIt == this->_components.end()) {
            return std::nullopt;
        }

        return componentIt->second;
    }
}
