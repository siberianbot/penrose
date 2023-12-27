#include "EntityManagerImpl.hpp"

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Utils/TypeUtils.hpp>

#include "src/Utils/SyncUtils.hpp"

namespace Penrose {

    EntityManagerImpl::EntityManagerImpl(const ResourceSet *resources)
        : _resources(resources),
          _log(resources->get<Log>()),
          _eventQueue(resources->get<ECSEventQueue>()),
          _semaphore(1),
          _refCount(0) {
        //
    }

    void EntityManagerImpl::init() {
        // nothing to do
    }

    void EntityManagerImpl::destroy() {
        this->_entities.reset();
    }

    Entity EntityManagerImpl::createEntity() {
        const auto guard = SemaphoreGuard(this->_semaphore);

        return this->_entities.acquire();
    }

    Entity EntityManagerImpl::createEntity(std::string_view &&archetype, const Params &params) {
        const auto it = this->_archetypes.find(std::string(archetype));

        if (it == this->_archetypes.end()) {
            throw EngineError("Entity archetype {} not found", archetype);
        }

        const auto newComponents = it->second->construct(params);

        const auto guard = SemaphoreGuard(this->_semaphore);

        const auto entity = this->_entities.acquire();
        auto &components = this->_entities.get(entity);

        for (const auto &componentPtr: newComponents) {
            components.insert_or_assign(componentPtr->getType(), componentPtr);
        }

        return entity;
    }

    void EntityManagerImpl::destroyEntity(const Entity entity) {
        const auto guard = SemaphoreGuard(this->_semaphore);

        this->_entities.release(entity);
    }

    void EntityManagerImpl::addComponent(const Entity entity, std::shared_ptr<ComponentPtr> &&component) {
        const auto guard = SemaphoreGuard(this->_semaphore);

        auto &components = this->_entities.get(entity);

        if (components.contains(component->getType())) {
            throw EngineError("Entity {} already have component {}", entity, demangle(component->getType().name()));
        }

        components.emplace(component->getType(), std::forward<decltype(component)>(component));
    }

    void EntityManagerImpl::removeComponent(const Entity entity, ComponentType &&componentType) {
        const auto guard = SemaphoreGuard(this->_semaphore);

        auto &components = this->_entities.get(entity);

        if (!components.contains(componentType)) {
            throw EngineError("Entity {} does not have component {}", entity, demangle(componentType.name()));
        }

        components.erase(componentType);
    }

    std::optional<std::shared_ptr<ComponentPtr>> EntityManagerImpl::tryGetComponent(
        const Entity entity, ComponentType &&componentType
    ) {
        const auto guard = SemaphoreGuard(this->_semaphore);

        const auto &components = this->_entities.get(entity);
        const auto it = components.find(componentType);

        if (it == components.end()) {
            return std::nullopt;
        }

        return it->second;
    }

    std::shared_ptr<ComponentPtr> EntityManagerImpl::getComponent(const Entity entity, ComponentType &&componentType) {
        const auto guard = SemaphoreGuard(this->_semaphore);
        const auto component = this->tryGetComponent(entity, ComponentType(componentType));

        if (!component.has_value()) {
            throw EngineError("Entity {} does not have component {}", entity, demangle(componentType.name()));
        }

        return *component;
    }

    std::unique_ptr<EntityIterator> EntityManagerImpl::iterate() {
        return std::make_unique<AllIterator>(this);
    }

    EntityQuery EntityManagerImpl::query() {
        return EntityQuery(this->iterate());
    }

    void EntityManagerImpl::addArchetype(std::type_index &&type) {
        const auto archetype = this->_resources->resolveOne<EntityArchetype>(std::forward<decltype(type)>(type));

        if (this->_archetypes.contains(archetype->getName())) {
            throw EngineError("Entity archetype {} already added into entity manager", archetype->getName());
        }

        this->_archetypes.emplace(archetype->getName(), archetype);
    }

    EntityManagerImpl::AllIterator::AllIterator(EntityManagerImpl *entityManager)
        : _entityManager(entityManager) {
        this->_entityManager->acquireLock();
    }

    EntityManagerImpl::AllIterator::~AllIterator() {
        this->_entityManager->releaseLock();
    }

    bool EntityManagerImpl::AllIterator::move() {
        if (!this->_currentEntity.has_value() && this->proceedEntity() && this->proceedComponent()) {
            return true;
        }

        if (!this->proceedComponent() && this->proceedEntity() && this->proceedComponent()) {
            return true;
        }

        return false;
    }

    EntityEntry EntityManagerImpl::AllIterator::fetch() {
        return {
            .entity = *this->_currentEntity,
            .componentType = (*this->_currentComponent)->first,
            .component = (*this->_currentComponent)->second,
        };
    }

    bool EntityManagerImpl::AllIterator::proceedComponent() {
        auto &components = this->_entityManager->_entities.get(*this->_currentEntity);

        if (!this->_currentComponent.has_value()) {
            this->_currentComponent = components.begin();
        } else if (*this->_currentComponent != components.end()) {
            ++*this->_currentComponent;
        }

        return *this->_currentComponent != components.end();
    }

    bool EntityManagerImpl::AllIterator::proceedEntity() {
        auto entity = this->_currentEntity.value_or(0);

        if (!this->_currentEntity.has_value()) {
            entity = 0;
        } else {
            entity++;
        }

        while (entity < this->_entityManager->_entities.size()) {
            if (this->_entityManager->_entities.acquired(entity)
                && !this->_entityManager->_entities.get(entity).empty()) {
                this->_currentEntity = entity;
                this->_currentComponent = std::nullopt;

                return true;
            }

            entity++;
        }

        return false;
    }

    void EntityManagerImpl::acquireLock() {
        if (this->_refCount == 0) {
            this->_semaphore.acquire();
        }

        ++this->_refCount;
    }

    void EntityManagerImpl::releaseLock() {
        --this->_refCount;

        if (this->_refCount == 0) {
            this->_semaphore.release();
        }
    }
}
