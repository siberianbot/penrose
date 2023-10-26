#ifndef PENROSE_ECS_ENTITY_MANAGER_HPP
#define PENROSE_ECS_ENTITY_MANAGER_HPP

#include <atomic>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <typeindex>
#include <vector>

#include <Penrose/Api.hpp>
#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Common/Log.hpp>
#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Events/ECSEvents.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

namespace Penrose {

    class PENROSE_API EntityManager : public Resource<EntityManager, ResourceGroup::ECSManager>,
                                      public Initializable {
    public:
        struct Entry {
            Entity entity;
            ComponentInfo type;
            std::shared_ptr<ComponentBase> component;
        };

        class Iterator {
        public:
            virtual ~Iterator() = default;

            [[nodiscard]] virtual bool move() = 0;
            [[nodiscard]] virtual Entry fetch() = 0;
        };

        class Query {
        public:
            explicit Query(EntityManager *entityManager);

            [[nodiscard]] Query &entity(Entity entity);
            [[nodiscard]] Query &component(ComponentInfo &&componentType);

            template<typename C>
            requires std::is_base_of_v<Component<C>, C>
            [[nodiscard]] Query &component() {
                return this->component(C::type());
            }

            [[nodiscard]] std::optional<Entry> front();
            [[nodiscard]] std::list<Entry> all();

        private:
            EntityManager *_entityManager;

            std::unique_ptr<Iterator> _iterator;
        };

        explicit EntityManager(ResourceSet *resources);
        ~EntityManager() override = default;

        void init() override { /* nothing to do */ }

        void destroy() override;

        [[nodiscard]] Entity createEntity();
        void destroyEntity(Entity entity);

        void addComponent(Entity entity, std::shared_ptr<ComponentBase> &&component);
        void removeComponent(Entity entity, ComponentInfo &&componentType);

        [[nodiscard]] std::optional<std::shared_ptr<ComponentBase>> tryGetComponent(Entity entity,
                                                                                    ComponentInfo &&componentType);

        template<typename C>
        requires std::is_base_of_v<Component<C>, C>
        void removeComponent(Entity entity) {
            this->removeComponent(entity, C::type());
        }

        template<typename C>
        requires std::is_base_of_v<Component<C>, C>
        [[nodiscard]] bool hasComponent(Entity entity) {
            return this->tryGetComponent(entity, C::type()).has_value();
        }

        template<typename C>
        requires std::is_base_of_v<Component<C>, C>
        [[nodiscard]] std::optional<std::shared_ptr<C>> tryGetComponent(Entity entity) {
            return map(
                    this->tryGetComponent(entity, C::type()),
                    [](const std::shared_ptr<ComponentBase> &component) {
                        return std::dynamic_pointer_cast<C>(component);
                    });
        }

        template<typename C>
        requires std::is_base_of_v<Component<C>, C>
        [[nodiscard]] std::shared_ptr<C> getComponent(Entity entity) {
            return orElseThrow(
                    this->tryGetComponent<C>(entity),
                    EngineError("Entity {} does not have component {}", entity, C::type().name));
        }

        [[nodiscard]] Query query();

    private:
        using ComponentCollection = std::map<std::type_index, std::shared_ptr<ComponentBase>>;

        struct EntityData {
            bool acquired;
            ComponentCollection components;
        };

        using EntityCollection = std::vector<EntityData>;

        class AllIterator : public Iterator {
        public:
            explicit AllIterator(EntityCollection *entities);
            ~AllIterator() override = default;

            [[nodiscard]] bool move() override;
            [[nodiscard]] Entry fetch() override;

        private:
            EntityCollection *_entities;
            std::optional<Entity> _entity;
            std::optional<ComponentCollection::const_iterator> _componentIterator;
        };

        class EntityIterator : public Iterator {
        public:
            explicit EntityIterator(std::unique_ptr<Iterator> &&next);
            ~EntityIterator() override = default;

            void entity(Entity entity);

            [[nodiscard]] bool move() override;
            [[nodiscard]] Entry fetch() override;

        private:
            std::unique_ptr<Iterator> _next;
            std::set<Entity> _entities;
        };

        class ComponentIterator : public Iterator {
        public:
            explicit ComponentIterator(std::unique_ptr<Iterator> &&next);
            ~ComponentIterator() override = default;

            void component(std::type_index type);

            [[nodiscard]] bool move() override;
            [[nodiscard]] Entry fetch() override;

        private:
            std::unique_ptr<Iterator> _next;
            std::set<std::type_index> _types;
        };

        ResourceProxy<ECSEventQueue> _ecsEventQueue;
        ResourceProxy<Log> _log;

        std::atomic_uint32_t _entitiesAcquiredCount;
        EntityCollection _entities;

        void resize(std::uint32_t size);

        [[nodiscard]] std::optional<Entity> tryAcquireEntity();
        [[nodiscard]] std::optional<EntityData *> tryGetEntityData(Entity entity);
    };
}

#endif // PENROSE_ECS_ENTITY_MANAGER_HPP
