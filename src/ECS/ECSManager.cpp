#include <Penrose/ECS/ECSManager.hpp>

#include <fmt/core.h>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    ECSManager::ECSManager(ResourceSet *resources)
            : _resources(resources),
              _eventQueue(resources->get<EventQueue>()) {
        //
    }

    void ECSManager::init() {
        for (auto &[_, entry]: this->_systems) {
            if (!entry.activateImmediately || entry.enabled) {
                continue;
            }

            entry.instance->init();
            entry.enabled = true;
        }
    }

    void ECSManager::destroy() {
        for (auto &[_, entry]: this->_systems) {
            if (!entry.enabled) {
                continue;
            }

            entry.instance->destroy();
            entry.enabled = false;
        }

        this->_entities.clear();
    }

    Entity ECSManager::createEntity() {
        auto entity = this->_nextEntity++;
        this->_entities.emplace(entity, EntityEntry{});

        auto event = makeEvent(EventType::EntityCreated, EntityEventValue{
                .entity = entity
        });
        this->_eventQueue->push(event);

        return entity;
    }

    void ECSManager::destroyEntity(Entity &&entity) {
        auto it = this->_entities.find(entity);

        if (it == this->_entities.end()) {
            // TODO: notify
            return;
        }

        this->_entities.erase(it);

        auto event = makeEvent(EventType::EntityDestroyed, EntityEventValue{
                .entity = entity
        });
        this->_eventQueue->push(event);
    }

    std::vector<std::shared_ptr<Component>> ECSManager::queryEntity(const Entity &entity) const {
        auto entityIt = this->_entities.find(entity);
        if (entityIt == this->_entities.end()) {
            throw EngineError(fmt::format("Entity {} not found", entity));
        }

        auto result = std::vector<std::shared_ptr<Component>>(entityIt->second.components.size());
        auto idx = 0;

        for (const auto &[name, component]: entityIt->second.components) {
            result[idx++] = component;
        }

        return result;
    }

    void ECSManager::updateSystems(float delta) {
        for (const auto &[_, entry]: this->_systems) {
            if (!entry.enabled) {
                continue;
            }

            entry.instance->update(delta);
        }
    }

    std::shared_ptr<Component> ECSManager::createComponent(const std::string &name) const {
        auto it = this->_componentFactories.find(name);

        if (it == this->_componentFactories.end()) {
            throw EngineError(fmt::format("Unknown component {}", name));
        }

        return it->second();
    }

    void ECSManager::addComponent(const Entity &entity, std::string &&name,
                                  const std::shared_ptr<Component> &instance) {
        auto it = this->_entities.find(entity);

        if (it == this->_entities.end()) {
            throw EngineError(fmt::format("Entity {} not found", entity));
        }

        if (it->second.components.find(name) != it->second.components.end()) {
            throw EngineError(fmt::format("Entity {} already have component {}", entity, name));
        }

        it->second.components[name] = instance;

        auto event = makeEvent(EventType::ComponentCreated, ComponentEventValue{
                .entity = entity,
                .componentName = name
        });
        this->_eventQueue->push(event);
    }

    void ECSManager::removeComponent(const Entity &entity, std::string &&name) {
        auto entityIt = this->_entities.find(entity);
        if (entityIt == this->_entities.end()) {
            throw EngineError(fmt::format("Entity {} not found", entity));
        }

        auto componentIt = entityIt->second.components.find(name);
        if (componentIt == entityIt->second.components.end()) {
            throw EngineError(fmt::format("Entity {} does not have component {}", entity, name));
        }

        entityIt->second.components.erase(componentIt);

        auto event = makeEvent(EventType::ComponentDestroyed, ComponentEventValue{
                .entity = entity,
                .componentName = name
        });
        this->_eventQueue->push(event);
    }

    std::optional<std::shared_ptr<Component>> ECSManager::tryGetComponent(const Entity &entity,
                                                                          const std::string &name) const {
        auto entityIt = this->_entities.find(entity);
        if (entityIt == this->_entities.end()) {
            throw EngineError(fmt::format("Entity {} not found", entity));
        }

        auto componentIt = entityIt->second.components.find(name);
        if (componentIt == entityIt->second.components.end()) {
            return std::nullopt;
        }

        return componentIt->second;
    }

    std::vector<Entity> ECSManager::queryComponents(const std::string &name) const {
        std::vector<Entity> result;

        for (const auto &[entity, entry]: this->_entities) {
            auto it = entry.components.find(name);

            if (it == entry.components.end()) {
                continue;
            }

            result.push_back(entity);
        }

        return result;
    }

    void ECSManager::enableSystem(std::string &&name) {
        auto it = this->_systems.find(name);

        if (it == this->_systems.end()) {
            throw EngineError(fmt::format("Unknown system {}", name));
        }

        if (it->second.enabled) {
            return;
        }

        it->second.instance->init();
        it->second.enabled = true;
    }

    void ECSManager::disableSystem(std::string &&name) {
        auto it = this->_systems.find(name);

        if (it == this->_systems.end()) {
            throw EngineError(fmt::format("Unknown system {}", name));
        }

        if (!it->second.enabled) {
            return;
        }

        it->second.instance->destroy();
        it->second.enabled = false;
    }
}
