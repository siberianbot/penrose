#ifndef PENROSE_ECS_ECS_MANAGER_HPP
#define PENROSE_ECS_ECS_MANAGER_HPP

#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <Penrose/Common/Initializable.hpp>
#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ECSBase.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Resources/Resource.hpp>


namespace Penrose {

    class ResourceSet;
    class EventQueue;

    template<typename T>
    concept IsComponent = std::is_base_of<Component, T>::value &&
                          std::is_default_constructible<T>::value &&
                          requires(T) {{ T::name() } -> std::same_as<ComponentName>; };

    template<typename T>
    concept IsSystem = std::is_base_of<System, T>::value && requires(ResourceSet *resources, T) {
        { T(resources) };
    };

    class ECSManager : public Resource, public Initializable {
    private:
        using ComponentType = std::uint32_t;
        using ComponentFactory = std::function<std::shared_ptr<Component>()>;

        struct ComponentTypeDescription {
            ComponentType type;
            ComponentName name;
            ComponentFactory factory;
        };

        struct EntityDescription {
            std::unordered_set<ComponentType> componentTypes;
        };

        ResourceSet *_resources;
        EventQueue *_eventQueue;

        Entity _nextEntity = 0;
        ComponentType _nextComponentType = 0;
        std::vector<ComponentTypeDescription> _componentTypes;
        std::unordered_map<std::type_index, std::unique_ptr<System>> _systems;
        std::unordered_map<Entity, EntityDescription> _entities;
        std::unordered_map<ComponentId, std::shared_ptr<Component>> _components;

        [[nodiscard]] std::optional<ComponentTypeDescription *> tryGetComponentTypeDescription(
                const ComponentName &name) const;
        [[nodiscard]] ComponentTypeDescription *getComponentTypeDescription(const ComponentName &name) const;
        [[nodiscard]] ComponentTypeDescription *getComponentTypeDescription(const ComponentType &type) const;

        [[nodiscard]] static ComponentId constructComponentId(const Entity &entity, const ComponentType &type);
        [[nodiscard]] static std::tuple<Entity, ComponentType> deconstructComponentId(const ComponentId &componentId);

    public:
        explicit ECSManager(ResourceSet *resources);
        ~ECSManager() override = default;

        void init() override;
        void update();
        void destroy() override;

        template<IsComponent T>
        void registerComponent();

        template<IsSystem T>
        void registerSystem();

        [[nodiscard]] Entity createEntity();
        void destroyEntity(const Entity &entity);

        [[nodiscard]] ComponentId createComponent(const Entity &entity, const ComponentName &name);
        void destroyComponent(const ComponentId &componentId);

        [[nodiscard]] std::optional<std::shared_ptr<Component>> tryGetComponent(const Entity &entity,
                                                                                const ComponentName &name) const;
        [[nodiscard]] std::optional<std::shared_ptr<Component>> tryGetComponent(const ComponentId &componentId) const;

        template<IsComponent T>
        [[nodiscard]] std::optional<std::shared_ptr<T>> tryGetComponent(const Entity &entity) const;

        template<IsComponent T>
        [[nodiscard]] std::shared_ptr<T> getComponent(const Entity &entity) const;
    };
}

#include "ECSManager.inl"

#endif // PENROSE_ECS_ECS_MANAGER_HPP
