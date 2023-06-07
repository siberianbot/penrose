#ifndef PENROSE_ECS_ECS_MANAGER_HPP
#define PENROSE_ECS_ECS_MANAGER_HPP

#include <functional>
#include <list>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

#include "src/Common/Initializable.hpp"
#include "src/ECS/ECSBase.hpp"
#include "src/Resources/Resource.hpp"

namespace Penrose {

    class ResourceSet;
    class EventQueue;

    using ComponentFactory = std::function<std::shared_ptr<Component>()>;

    struct ComponentTypeDescription {
        std::string name;
        ComponentFactory factory;
    };

    struct EntityDescription {
        std::unordered_set<ComponentType> componentTypes;
    };

    class ECSManager : public Resource, public Initializable {
    private:
        EventQueue *_eventQueue;

        Entity _nextEntity = 0;
        ComponentType _nextComponentType = 0;
        std::unordered_map<ComponentType, ComponentTypeDescription> _componentTypes;
        std::unordered_map<Entity, EntityDescription> _entities;
        std::unordered_map<ComponentId, std::shared_ptr<Component>> _components;

    public:
        explicit ECSManager(ResourceSet *resources);
        ~ECSManager() override = default;

        void init() override { /* nothing to do */ }

        void destroy() override;

        [[nodiscard]] ComponentType createComponentType(std::string_view &&name, ComponentFactory &&factory);

        [[nodiscard]] Entity createEntity();
        void destroyEntity(const Entity &entity);

        [[nodiscard]] ComponentId createComponent(const Entity &entity, const ComponentType &componentType);
        void destroyComponent(const ComponentId &componentId);

        [[nodiscard]] std::optional<std::weak_ptr<Component>> tryGetComponent(const Entity &entity,
                                                                              const ComponentType &componentType);
        [[nodiscard]] std::optional<std::weak_ptr<Component>> tryGetComponent(const ComponentId &componentId);
    };
}

#endif // PENROSE_ECS_ECS_MANAGER_HPP
