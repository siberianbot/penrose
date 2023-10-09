#include <Penrose/ECS/ECSManager.hpp>

#include <ranges>
#include <set>
#include <string_view>
#include <utility>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Events/ECSEvent.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

namespace Penrose {

    constexpr static const std::string_view ECS_MANAGER_TAG = "ECSManager";

    ECSManager::ECSManager(ResourceSet *resources)
            : _eventQueue(resources->getLazy<EventQueue>()),
              _log(resources->getLazy<Log>()),
              _componentFactories(resources->getAllLazy<ComponentFactory>()),
              _systems(resources->getAllLazy<System>()) {
        //
    }

    void ECSManager::init() {
        for (const auto &system: this->_systems) {
            this->_systemsMap.emplace(system->getName(), system);
        }
    }

    void ECSManager::destroy() {
        this->_entitiesAllocMap.reset();

        for (auto &entry: this->_entitiesEntries) {
            entry.components.clear();
        }

        this->_componentFactoriesMap.clear();
    }

    void ECSManager::update(float delta) {
        for (const auto &system: this->_systems) {
            system->update(delta);
        }
    }

    Entity ECSManager::createEntity() {
        std::optional<Entity> entity;

        for (std::uint32_t idx = 0; idx < this->_entitiesAllocMap.size(); idx++) {
            if (this->_entitiesAllocMap.test(idx)) {
                continue;
            }

            entity = idx;
        }

        if (!entity.has_value()) {
            throw EngineError("Entity allocation failed");
        }

        this->_entitiesEntries[*entity] = {};
        this->_entitiesAllocMap.set(*entity, true);

        auto data = ECSEventArgs{
                .type = ECSEventType::EntityCreated,
                .entity = *entity,
                .componentName = "",
                .component = nullptr
        };

        this->_eventQueue->pushEvent<EventType::ECSEvent>(data);

        return *entity;
    }

    void ECSManager::destroyEntity(Entity &&entity) {
        auto entry = this->tryGetEntity(entity);

        if (!entry.has_value()) {
            return;
        }

        this->_entitiesAllocMap.set(entity, false);
        (*entry)->components.clear();

        auto data = ECSEventArgs{
                .type = ECSEventType::EntityDestroyed,
                .entity = entity,
                .componentName = "",
                .component = nullptr
        };

        this->_eventQueue->pushEvent<EventType::ECSEvent>(data);
    }

    const ECSManager::ComponentMap &ECSManager::getComponents(const Entity &entity) {
        auto entry = this->tryGetEntity(entity);

        if (!entry.has_value()) {
            throw EngineError("Entity required");
        }

        return (*entry)->components;
    }

    std::vector<ECSEntry> ECSManager::query(const ECSQuery &query) {
        std::vector<ECSEntry> entries;

        for (std::uint32_t idx = 0; idx < this->_entitiesEntries.size(); ++idx) {
            if (!this->_entitiesAllocMap.test(idx)) {
                continue;
            }

            auto entityEntry = this->_entitiesEntries.at(idx);

            for (const auto &[componentName, component]: entityEntry.components) {
                auto ecsEntry = ECSEntry{
                        .entity = idx,
                        .componentName = componentName,
                        .component = component
                };

                bool matched = true;

                for (const auto &predicate: query.getPredicates()) {
                    if (!predicate(ecsEntry)) {
                        matched = false;

                        break;
                    }
                }

                if (matched) {
                    entries.push_back(std::forward<decltype(ecsEntry)>(ecsEntry));
                }
            }
        }

        return std::forward<decltype(entries)>(entries);
    }

    std::shared_ptr<Component> ECSManager::makeComponent(const std::string &name) {
        if (this->_componentFactoriesMap.empty()) {
            for (const auto &factory: this->_componentFactories) {
                this->_componentFactoriesMap.emplace(factory->getName(), factory);
            }
        }

        auto it = this->_componentFactoriesMap.find(name);

        if (it == this->_componentFactoriesMap.end()) {
            throw EngineError("Unknown component {}", name);
        }

        return std::shared_ptr<Component>(it->second->makeComponent());
    }

    std::optional<ECSManager::EntityEntry *> ECSManager::tryGetEntity(const Entity &entity) {
        if (entity >= ALLOC_SIZE) {
            throw EngineError("Invalid entity: greater than ALLOC_SIZE");
        }

        if (!this->_entitiesAllocMap.test(entity)) {
            this->_log->writeError(ECS_MANAGER_TAG, "Entity {} wasn't allocated", entity);

            return std::nullopt;
        }

        return &this->_entitiesEntries.at(entity);
    }

    void ECSManager::addComponent(const Entity &entity, std::string &&name, std::shared_ptr<Component> &&instance) {
        auto entry = this->tryGetEntity(entity);

        if (!entry.has_value()) {
            throw EngineError("Entity required");
        }

        auto componentIt = (*entry)->components.find(name);

        if (componentIt != (*entry)->components.end()) {
            throw EngineError("Entity {} already have component {}", entity, name);
        }

        (*entry)->components.emplace(name, instance);

        auto data = ECSEventArgs{
                .type = ECSEventType::ComponentCreated,
                .entity = entity,
                .componentName = name,
                .component = instance
        };

        this->_eventQueue->pushEvent<EventType::ECSEvent>(data);
    }

    std::optional<std::shared_ptr<Component>> ECSManager::tryGetComponent(const Entity &entity,
                                                                          const std::string &name) {
        auto entry = this->tryGetEntity(entity);

        if (!entry.has_value()) {
            throw EngineError("Entity required");
        }

        return tryGet((*entry)->components, name);
    }

    std::shared_ptr<Component> ECSManager::getComponent(const Entity &entity, std::string &&name) {
        return orElseThrow(
                this->tryGetComponent(entity, std::forward<decltype(name)>(name)),
                EngineError("Entity {} does not have component {}", entity, name)
        );
    }

    void ECSManager::removeComponent(const Entity &entity, std::string &&name) {
        auto entry = this->tryGetEntity(entity);

        if (!entry.has_value()) {
            return;
        }

        auto componentIt = (*entry)->components.find(name);
        if (componentIt == (*entry)->components.end()) {
            this->_log->writeError(ECS_MANAGER_TAG, "Entity {} does not have component {}", entity, name);

            return;
        }

        (*entry)->components.erase(componentIt);

        auto data = ECSEventArgs{
                .type = ECSEventType::ComponentDestroyed,
                .entity = entity,
                .componentName = name,
                .component = nullptr
        };

        this->_eventQueue->pushEvent<EventType::ECSEvent>(data);
    }
}
