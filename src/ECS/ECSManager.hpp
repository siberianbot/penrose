#ifndef PENROSE_ECS_ECS_MANAGER_HPP
#define PENROSE_ECS_ECS_MANAGER_HPP

#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "src/Common/Initializable.hpp"
#include "src/ECS/ECSBase.hpp"
#include "src/ECS/Components/Component.hpp"
#include "src/Resources/Resource.hpp"

namespace Penrose {

    class ResourceSet;
    class EventQueue;

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

        EventQueue *_eventQueue;

        Entity _nextEntity = 0;
        ComponentType _nextComponentType = 0;
        std::vector<ComponentTypeDescription> _componentTypes;
        std::unordered_map<Entity, EntityDescription> _entities;
        std::unordered_map<ComponentId, std::shared_ptr<Component>> _components;

        [[nodiscard]] std::optional<ComponentTypeDescription *> tryGetComponentTypeDescription(
                const ComponentName &name);
        [[nodiscard]] ComponentTypeDescription *getComponentTypeDescription(const ComponentName &name);
        [[nodiscard]] ComponentTypeDescription *getComponentTypeDescription(const ComponentType &type);

        [[nodiscard]] static ComponentId constructComponentId(const Entity &entity, const ComponentType &type);
        [[nodiscard]] static std::tuple<Entity, ComponentType> deconstructComponentId(const ComponentId &componentId);

    public:
        explicit ECSManager(ResourceSet *resources);
        ~ECSManager() override = default;

        void init() override { /* nothing to do */ }

        void destroy() override;

        template<IsComponent T>
        void registerComponent();

        [[nodiscard]] Entity createEntity();
        void destroyEntity(const Entity &entity);

        [[nodiscard]] ComponentId createComponent(const Entity &entity, const ComponentName &name);
        void destroyComponent(const ComponentId &componentId);

        [[nodiscard]] std::optional<std::weak_ptr<Component>> tryGetComponent(const Entity &entity,
                                                                              const ComponentName &name);
        [[nodiscard]] std::optional<std::weak_ptr<Component>> tryGetComponent(const ComponentId &componentId);

        template<IsComponent T>
        [[nodiscard]] std::optional<std::weak_ptr<T>> tryGetComponent(const Entity &entity);
    };
}

#include "ECSManager.inl"

#endif // PENROSE_ECS_ECS_MANAGER_HPP
