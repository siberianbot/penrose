#include <Penrose/ECS/EntityManager.hpp>

#include <string_view>

namespace Penrose {

    constexpr static const std::string_view ENTITY_MANAGER_TAG = "EntityManager";

    EntityManager::Query::Query(EntityManager *entityManager)
        : _entityManager(entityManager),
          _iterator(std::make_unique<AllIterator>(&this->_entityManager->_entities)) {
        //
    }

    EntityManager::Query &EntityManager::Query::entity(Entity entity) {

        auto entityIterator = dynamic_cast<EntityIterator *>(this->_iterator.get());

        if (entityIterator == nullptr) {
            this->_iterator = std::make_unique<EntityIterator>(std::move(this->_iterator));

            entityIterator = dynamic_cast<EntityIterator *>(this->_iterator.get());
        }

        entityIterator->entity(entity);

        return *this;
    }

    EntityManager::Query &EntityManager::Query::component(ComponentInfo &&componentType) {

        auto componentIterator = dynamic_cast<ComponentIterator *>(this->_iterator.get());

        if (componentIterator == nullptr) {
            this->_iterator = std::make_unique<ComponentIterator>(std::move(this->_iterator));

            componentIterator = dynamic_cast<ComponentIterator *>(this->_iterator.get());
        }

        componentIterator->component(componentType.type);

        return *this;
    }

    std::optional<EntityManager::Entry> EntityManager::Query::front() {
        if (!this->_iterator->move()) {
            return std::nullopt;
        }

        return this->_iterator->fetch();
    }

    std::list<EntityManager::Entry> EntityManager::Query::all() {
        std::list<Entry> result;

        while (this->_iterator->move()) {
            result.emplace_back(this->_iterator->fetch());
        }

        return result;
    }

    EntityManager::EntityManager(const ResourceSet *resources)
        : _ecsEventQueue(resources->get<ECSEventQueue>()),
          _log(resources->get<Log>()) {
        //
    }

    void EntityManager::destroy() {
        this->_entitiesAcquiredCount = 0;
        this->_entities.clear();
    }

    Entity EntityManager::createEntity() {
        auto maybeEntity = this->tryAcquireEntity();

        if (!maybeEntity.has_value()) {
            throw EngineError("Entity allocation failed");
        }

        this->_ecsEventQueue->push(EntityCreatedEvent {
            .entity = *maybeEntity,
        });

        return *maybeEntity;
    }

    void EntityManager::destroyEntity(Entity entity) {
        auto maybeData = this->tryGetEntityData(entity);

        if (!maybeData.has_value()) {
            this->_log->writeError(ENTITY_MANAGER_TAG, "Entity {:#x} was not acquired", entity);

            return;
        }

        for (const auto &[_, component]: (*maybeData)->components) {
            this->_ecsEventQueue->push(ComponentDestroyedEvent {
                .entity = entity,
                .componentType = component->getType(),
            });
        }

        (*maybeData)->acquired = false;
        (*maybeData)->components.clear();

        this->_entitiesAcquiredCount--;

        this->_ecsEventQueue->push(EntityDestroyedEvent {
            .entity = entity,
        });
    }

    void EntityManager::addComponent(Entity entity, std::shared_ptr<ComponentBase> &&component) {
        auto maybeData = this->tryGetEntityData(entity);

        if (!maybeData.has_value()) {
            throw EngineError("Entity {:#x} was not acquired", entity);
        }

        auto type = component->getType();

        if ((*maybeData)->components.contains(type.type)) {
            throw EngineError("Entity {:#x} already contains component {}", entity, type.name);
        }

        (*maybeData)->components.emplace(type.type, std::shared_ptr(component));

        this->_ecsEventQueue->push(ComponentCreatedEvent {
            .entity = entity,
            .componentType = component->getType(),
            .component = std::shared_ptr(component),
        });
    }

    void EntityManager::removeComponent(Entity entity, ComponentInfo &&componentType) {
        auto maybeData = this->tryGetEntityData(entity);

        if (!maybeData.has_value()) {
            this->_log->writeError(ENTITY_MANAGER_TAG, "Entity {:#x} was not acquired", entity);

            return;
        }

        auto it = (*maybeData)->components.find(componentType.type);

        if (it == (*maybeData)->components.end()) {
            this->_log->writeError(
                ENTITY_MANAGER_TAG, "Entity {:#x} does not have component {}", entity, componentType.name
            );
            return;
        }

        (*maybeData)->components.erase(it);
    }

    std::optional<std::shared_ptr<ComponentBase>> EntityManager::tryGetComponent(
        Entity entity, ComponentInfo &&componentType
    ) {
        auto maybeData = this->tryGetEntityData(entity);

        if (!maybeData.has_value()) {
            this->_log->writeError(ENTITY_MANAGER_TAG, "Entity {:#x} was not acquired", entity);

            return std::nullopt;
        }

        return tryGet((*maybeData)->components, componentType.type);
    }

    EntityManager::Query EntityManager::query() {
        return EntityManager::Query(this);
    }

    void EntityManager::resize(std::uint32_t size) {
        auto emptyData = EntityData {.acquired = false, .components = {}};

        this->_entities.resize(size, emptyData);
    }

    std::optional<Entity> EntityManager::tryAcquireEntity() {
        if (this->_entitiesAcquiredCount == this->_entities.size()) {
            this->resize(std::max<std::uint32_t>(2 * this->_entities.size(), 64 * 1024));
        }

        std::optional<Entity> entity;

        for (std::uint32_t e = 0; e < this->_entities.size(); ++e) {
            auto &data = this->_entities.at(e);

            if (data.acquired) {
                continue;
            }

            data.acquired = true;
            this->_entitiesAcquiredCount++;

            entity = e;

            break;
        }

        return entity;
    }

    std::optional<EntityManager::EntityData *> EntityManager::tryGetEntityData(Entity entity) {
        if (entity >= this->_entities.size()) {
            return std::nullopt;
        }

        auto &data = this->_entities.at(entity);

        if (!data.acquired) {
            return std::nullopt;
        }

        return &data;
    }

    EntityManager::AllIterator::AllIterator(EntityCollection *entities)
        : _entities(entities),
          _entity(std::nullopt),
          _componentIterator(std::nullopt) {
        //
    }

    bool EntityManager::AllIterator::move() {

        if (!this->_entity.has_value()) {
            this->_entity = 0;
        } else {
            (*this->_componentIterator)++;

            if (this->_componentIterator != this->_entities->at(*this->_entity).components.end()) {
                return true;
            }

            (*this->_entity)++;
        }

        while (this->_entity < this->_entities->size()
               && (!this->_entities->at(*this->_entity).acquired
                   || this->_entities->at(*this->_entity).components.empty())) {
            (*this->_entity)++;
        }

        if (this->_entity >= this->_entities->size()) {
            return false;
        }

        this->_componentIterator = this->_entities->at(*this->_entity).components.begin();

        return true;
    }

    EntityManager::Entry EntityManager::AllIterator::fetch() {
        auto [_, component] = (**this->_componentIterator);

        return Entry {.entity = (*this->_entity), .type = component->getType(), .component = component};
    }

    EntityManager::EntityIterator::EntityIterator(std::unique_ptr<Iterator> &&next)
        : _next(std::forward<decltype(next)>(next)) {
        //
    }

    void EntityManager::EntityIterator::entity(Entity entity) {
        this->_entities.insert(entity);
    }

    bool EntityManager::EntityIterator::move() {
        while (this->_next->move()) {
            auto currentEntity = this->_next->fetch().entity;

            if (this->_entities.contains(currentEntity)) {
                return true;
            }
        }

        return false;
    }

    EntityManager::Entry EntityManager::EntityIterator::fetch() {
        return this->_next->fetch();
    }

    EntityManager::ComponentIterator::ComponentIterator(std::unique_ptr<Iterator> &&next)
        : _next(std::forward<decltype(next)>(next)) {
        //
    }

    void EntityManager::ComponentIterator::component(std::type_index type) {
        this->_types.insert(type);
    }

    bool EntityManager::ComponentIterator::move() {
        while (this->_next->move()) {
            auto currentType = this->_next->fetch().type.type;

            if (this->_types.contains(currentType)) {
                return true;
            }
        }

        return false;
    }

    EntityManager::Entry EntityManager::ComponentIterator::fetch() {
        return this->_next->fetch();
    }
}
