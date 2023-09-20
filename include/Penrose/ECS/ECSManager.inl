#ifndef PENROSE_ECS_ECS_MANAGER_INL
#define PENROSE_ECS_ECS_MANAGER_INL

#include <utility>

namespace Penrose {

    template<IsComponent T>
    std::shared_ptr<T> ECSManager::addComponent(const Entity &entity) {
        auto name = std::string(T::name());
        auto component = this->makeComponent(name);

        this->addComponent(entity, std::forward<decltype(name)>(name), std::forward<decltype(component)>(component));

        return std::dynamic_pointer_cast<T>(component);
    }

    template<IsComponent T>
    std::shared_ptr<T> ECSManager::getComponent(const Entity &entity) {
        auto name = std::string(T::name());
        auto component = this->getComponent(entity, std::forward<decltype(name)>(name));

        return std::dynamic_pointer_cast<T>(component);
    }

    template<IsComponent T>
    void ECSManager::removeComponent(const Entity &entity) {
        auto name = std::string(T::name());

        this->removeComponent(entity, std::forward<decltype(name)>(name));
    }
}

#endif // PENROSE_ECS_ECS_MANAGER_INL
