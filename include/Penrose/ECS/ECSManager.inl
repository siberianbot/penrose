#ifndef PENROSE_ECS_ECS_MANAGER_INL
#define PENROSE_ECS_ECS_MANAGER_INL

#include <fmt/core.h>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

namespace Penrose {

    template<IsComponent T>
    void ECSManager::registerComponent() {
        auto name = T::name();

        if (this->tryGetComponentTypeDescription(name).has_value()) {
            throw EngineError(fmt::format("Component {} already registered", name));
        }

        auto description = ComponentTypeDescription{
                .type = this->_nextComponentType++,
                .name = name,
                .factory = []() {
                    return std::make_shared<T>();
                }
        };

        this->_componentTypes.push_back(description);
    }

    template<IsSystem T>
    void ECSManager::registerSystem() {
        auto type = std::type_index(typeid(T));

        if (this->_systems.find(type) != this->_systems.end()) {
            throw EngineError(fmt::format("System {} already registered", type.name()));
        }

        this->_systems[type] = std::make_unique<T>(this->_resources);
    }

    template<IsComponent T>
    std::optional<std::shared_ptr<T>> ECSManager::tryGetComponent(const Entity &entity) const {
        auto name = T::name();
        auto component = this->tryGetComponent(entity, name);

        if (!component.has_value()) {
            return std::nullopt;
        }

        return std::dynamic_pointer_cast<T>(component.value());
    }

    template<IsComponent T>
    std::shared_ptr<T> ECSManager::getComponent(const Entity &entity) const {
        return orElseThrow(this->tryGetComponent<T>(entity),
                           EngineError(fmt::format("No component {} found for entity {}", T::name(), entity)));
    }
}

#endif // PENROSE_ECS_ECS_MANAGER_INL