#ifndef PENROSE_ECS_ECS_MANAGER_INL
#define PENROSE_ECS_ECS_MANAGER_INL

#include <fmt/core.h>

#include "src/Common/EngineError.hpp"
#include "src/Utils/OptionalUtils.hpp"

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

    template<IsComponent T>
    std::optional<std::weak_ptr<T>> ECSManager::tryGetComponent(const Entity &entity) {
        auto name = T::name();
        auto component = this->tryGetComponent(entity, name);

        if (!component.has_value()) {
            return std::nullopt;
        }

        return std::dynamic_pointer_cast<T>(component.value().lock());
    }
}

#endif // PENROSE_ECS_ECS_MANAGER_INL
