#ifndef PENROSE_ECS_ENTITY_MANAGER_IMPL_HPP
#define PENROSE_ECS_ENTITY_MANAGER_IMPL_HPP

#include <atomic>
#include <map>
#include <memory>
#include <semaphore>
#include <vector>

#include <Penrose/Common/Log.hpp>
#include <Penrose/ECS/EntityIterator.hpp>
#include <Penrose/ECS/EntityManager.hpp>
#include <Penrose/Events/ECSEvents.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/ECS/EntityStore.hpp"

namespace Penrose {

    class EntityManagerImpl final: public Resource<EntityManagerImpl>,
                                   public Initializable,
                                   public EntityManager {
    public:
        explicit EntityManagerImpl(const ResourceSet *resources);
        ~EntityManagerImpl() override = default;

        void init() override;
        void destroy() override;

        [[nodiscard]] Entity createEntity() override;
        [[nodiscard]] Entity createEntity(std::string_view &&archetype, const Params &params) override;

        void destroyEntity(Entity entity) override;

        void addComponent(Entity entity, std::shared_ptr<ComponentPtr> &&component) override;

        void removeComponent(Entity entity, ComponentType &&componentType) override;

        [[nodiscard]] std::optional<std::shared_ptr<ComponentPtr>> tryGetComponent(
            Entity entity, ComponentType &&componentType
        ) override;

        [[nodiscard]] std::shared_ptr<ComponentPtr> getComponent(Entity entity, ComponentType &&componentType) override;

        [[nodiscard]] std::unique_ptr<EntityIterator> iterate() override;

        [[nodiscard]] EntityQuery query() override;

        void addArchetype(std::type_index &&type) override;

    private:
        class AllIterator final: public EntityIterator {
        public:
            explicit AllIterator(EntityManagerImpl *entityManager);
            ~AllIterator() override;

            [[nodiscard]] bool move() override;
            [[nodiscard]] EntityEntry fetch() override;

        private:
            EntityManagerImpl *_entityManager;

            std::optional<Entity> _currentEntity;
            std::optional<EntityStore::ComponentCollection::const_iterator> _currentComponent;

            [[nodiscard]] bool proceedComponent();
            [[nodiscard]] bool proceedEntity();
        };

        const ResourceSet *_resources;
        ResourceProxy<Log> _log;
        ResourceProxy<ECSEventQueue> _eventQueue;

        std::map<std::string, EntityArchetype *> _archetypes;

        std::binary_semaphore _iterationSemaphore;
        std::atomic_uint32_t _iterationRefCount;

        EntityStore _entities;

        void acquireIterationLock();
        void releaseIterationLock();
    };
}

#endif // PENROSE_ECS_ENTITY_MANAGER_IMPL_HPP
