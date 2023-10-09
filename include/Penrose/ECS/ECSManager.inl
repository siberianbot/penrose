#ifndef PENROSE_ECS_ECS_MANAGER_INL
#define PENROSE_ECS_ECS_MANAGER_INL

#include <utility>

#include <Penrose/Utils/OptionalUtils.hpp>

namespace Penrose {

    template<IsComponent T>
    std::shared_ptr<T> ECSManager::addComponent(const Entity &entity) {
        auto name = T::name();
        auto component = this->makeComponent(name);

        this->addComponent(entity, std::forward<decltype(name)>(name), std::forward<decltype(component)>(component));

        return std::dynamic_pointer_cast<T>(component);
    }

    template<IsComponent T>
    std::shared_ptr<T> ECSManager::getComponent(const Entity &entity) {
        auto name = T::name();
        auto component = this->getComponent(entity, std::forward<decltype(name)>(name));

        return std::dynamic_pointer_cast<T>(component);
    }

    template<IsComponent T>
    std::optional<std::shared_ptr<T>> ECSManager::tryGetComponent(const Entity &entity) {
        auto name = T::name();
        auto maybeComponent = this->tryGetComponent(entity, name);

        return map(maybeComponent, [](const std::shared_ptr<Component> &component) {
            return std::dynamic_pointer_cast<T>(component);
        });
    }

    template<IsComponent T>
    bool ECSManager::hasComponent(const Entity &entity) {
        return this->tryGetComponent<T>(entity).has_value();
    }

    template<IsComponent T>
    void ECSManager::removeComponent(const Entity &entity) {
        auto name = T::name();

        this->removeComponent(entity, std::forward<decltype(name)>(name));
    }
}

#endif // PENROSE_ECS_ECS_MANAGER_INL
