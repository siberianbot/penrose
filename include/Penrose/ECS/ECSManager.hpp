#ifndef PENROSE_ECS_ECS_MANAGER_HPP
#define PENROSE_ECS_ECS_MANAGER_HPP

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <Penrose/Common/Initializable.hpp>
#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/Entity.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;
    class EventQueue;

    class ECSManager : public Resource, public Initializable {
    public:
        explicit ECSManager(ResourceSet *resources);
        ~ECSManager() override = default;

        void init() override { /* nothing to do */ };
        void destroy() override;

        [[nodiscard]] Entity createEntity();
        void destroyEntity(Entity &&entity);

        template<IsComponent T>
        void registerComponent();

        template<IsComponent T>
        std::shared_ptr<T> addComponent(const Entity &entity);

        template<IsComponent T>
        void removeComponent(const Entity &entity);

        template<IsComponent T>
        [[nodiscard]] std::optional<std::shared_ptr<T>> tryGetComponent(const Entity &entity) const;

        template<IsComponent T>
        [[nodiscard]] std::shared_ptr<T> getComponent(const Entity &entity) const;

        template<IsComponent T>
        [[nodiscard]] std::vector<Entity> queryEntities() const;

        template<IsSystem T>
        void registerSystem();

        template<IsSystem T>
        void enableSystem();

        template<IsSystem T>
        void disableSystem();

        void updateSystems();

    private:
        using ComponentFactory = std::function<std::shared_ptr<Component>()>;

        struct SystemEntry {
            bool enabled;
            std::unique_ptr<System> instance;
        };

        struct EntityEntry {
            std::unordered_map<std::string, std::shared_ptr<Component>> components;
        };

        ResourceSet *_resources;
        EventQueue *_eventQueue;

        Entity _nextEntity = 0;
        std::unordered_map<std::string, ComponentFactory> _componentFactories;
        std::unordered_map<std::string, SystemEntry> _systems;
        std::unordered_map<Entity, EntityEntry> _entities;

        [[nodiscard]] std::shared_ptr<Component> createComponent(const std::string &name) const;

        void addComponent(const Entity &entity, std::string &&name, const std::shared_ptr<Component> &instance);
        void removeComponent(const Entity &entity, std::string &&name);

        [[nodiscard]] std::optional<std::shared_ptr<Component>> tryGetComponent(const Entity &entity,
                                                                                const std::string &name) const;
        [[nodiscard]] std::vector<Entity> queryEntitiesWithComponent(const std::string &name) const;

        void enableSystem(std::string &&name);
        void disableSystem(std::string &&name);
    };
}

#include "ECSManager.inl"

#endif // PENROSE_ECS_ECS_MANAGER_HPP
