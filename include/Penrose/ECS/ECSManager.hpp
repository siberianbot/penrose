#ifndef PENROSE_ECS_ECS_MANAGER_HPP
#define PENROSE_ECS_ECS_MANAGER_HPP

#include <bitset>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <Penrose/Api.hpp>
#include <Penrose/Common/Log.hpp>
#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>
#include <Penrose/ECS/ECSEntry.hpp>
#include <Penrose/ECS/ECSQuery.hpp>
#include <Penrose/ECS/Entity.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Events/ECSEvents.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Resources/Updatable.hpp>

namespace Penrose {

    class ResourceSet;

    class PENROSE_API ECSManager : public Resource<ECSManager>, public Initializable, public Updatable {
    public:
        constexpr static const std::size_t ALLOC_SIZE = 64 * 1024;

        using ComponentMap = std::map<std::string, std::shared_ptr<Component>>;

        explicit ECSManager(ResourceSet *resources);
        ~ECSManager() override = default;

        void init() override;
        void destroy() override;

        void update(float delta) override; // TODO: actually, every system can implement Updatable

        [[nodiscard]] Entity createEntity();
        void destroyEntity(Entity &&entity);

        template<IsComponent T>
        std::shared_ptr<T> addComponent(const Entity &entity);

        template<IsComponent T>
        [[nodiscard]] std::shared_ptr<T> getComponent(const Entity &entity);

        template<IsComponent T>
        [[nodiscard]] std::optional<std::shared_ptr<T>> tryGetComponent(const Entity &entity);

        template<IsComponent T>
        [[nodiscard]] bool hasComponent(const Entity &entity);

        const ComponentMap &getComponents(const Entity &entity);

        template<IsComponent T>
        void removeComponent(const Entity &entity);

        // TODO: VERY inefficient way to retrieve singular entity data
        [[nodiscard]] std::vector<ECSEntry> query(const ECSQuery &query);

    private:
        struct EntityEntry {
            ComponentMap components;
        };

        ResourceProxy<ECSEventQueue> _eventQueue;
        ResourceProxy<Log> _log;
        ResourceProxy<ComponentFactory> _componentFactories;
        ResourceProxy<System> _systems;

        std::map<std::string, ComponentFactory *> _componentFactoriesMap;
        std::map<std::string, System *> _systemsMap;

        std::bitset<ALLOC_SIZE> _entitiesAllocMap;
        std::array<EntityEntry, ALLOC_SIZE> _entitiesEntries;

        [[nodiscard]] std::shared_ptr<Component> makeComponent(const std::string &name);

        [[nodiscard]] std::optional<EntityEntry *> tryGetEntity(const Entity &entity);

        void addComponent(const Entity &entity, std::string &&name, const std::shared_ptr<Component> &instance);
        [[nodiscard]] std::optional<std::shared_ptr<Component>> tryGetComponent(const Entity &entity,
                                                                                const std::string &name);
        [[nodiscard]] std::shared_ptr<Component> getComponent(const Entity &entity, std::string &&name);
        void removeComponent(const Entity &entity, std::string &&name);
    };
}

#include "ECSManager.inl"

#endif // PENROSE_ECS_ECS_MANAGER_HPP
