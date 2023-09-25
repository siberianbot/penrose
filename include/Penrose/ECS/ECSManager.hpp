#ifndef PENROSE_ECS_ECS_MANAGER_HPP
#define PENROSE_ECS_ECS_MANAGER_HPP

#include <bitset>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <Penrose/Core/Log.hpp>
#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>
#include <Penrose/ECS/ECSEntry.hpp>
#include <Penrose/ECS/ECSQuery.hpp>
#include <Penrose/ECS/Entity.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/Updatable.hpp>

namespace Penrose {

    class ResourceSet;

    class ECSManager : public Resource, public Initializable, public Updatable {
    public:
        constexpr static const std::size_t ALLOC_SIZE = 64 * 1024;

        explicit ECSManager(ResourceSet *resources);
        ~ECSManager() override = default;

        void init() override;
        void destroy() override;

        void update(float delta) override;

        [[nodiscard]] Entity createEntity();
        void destroyEntity(Entity &&entity);

        template<IsComponent T>
        std::shared_ptr<T> addComponent(const Entity &entity);

        template<IsComponent T>
        std::shared_ptr<T> getComponent(const Entity &entity);

        template<IsComponent T>
        void removeComponent(const Entity &entity);

        [[nodiscard]] std::vector<ECSEntry> query(const ECSQuery &query);

    private:
        struct EntityEntry {
            std::map<std::string, std::shared_ptr<Component>> components;
        };

        Lazy<EventQueue> _eventQueue;
        Lazy<Log> _log;
        LazyCollection<ComponentFactory> _componentFactories;
        LazyCollection<System> _systems;

        std::map<std::string, ComponentFactory *> _componentFactoriesMap;
        std::map<std::string, System *> _systemsMap;

        std::bitset<ALLOC_SIZE> _entitiesAllocMap;
        std::array<EntityEntry, ALLOC_SIZE> _entitiesEntries;

        [[nodiscard]] std::shared_ptr<Component> makeComponent(const std::string &name);

        [[nodiscard]] std::optional<EntityEntry *> tryGetEntity(const Entity &entity);

        void addComponent(const Entity &entity, std::string &&name, std::shared_ptr<Component> &&instance);
        [[nodiscard]] std::shared_ptr<Component> getComponent(const Entity &entity, std::string &&name);
        void removeComponent(const Entity &entity, std::string &&name);
    };
}

#include "ECSManager.inl"

#endif // PENROSE_ECS_ECS_MANAGER_HPP
