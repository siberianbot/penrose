#ifndef PENROSE_ECS_ECS_MANAGER_INL
#define PENROSE_ECS_ECS_MANAGER_INL

#include <fmt/core.h>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

namespace Penrose {

    template<IsComponent T>
    void ECSManager::registerComponent() {
        auto name = std::string(T::name());

        if (this->_componentFactories.contains(name)) {
            throw EngineError(fmt::format("Component {} already registered", name));
        }

        this->_componentFactories.emplace(name, []() { return std::make_shared<T>(); });
    }

    template<IsComponent T>
    std::shared_ptr<T> ECSManager::addComponent(const Entity &entity) {
        auto name = std::string(T::name());
        auto component = this->createComponent(name);

        this->addComponent(entity, std::move(name), component);

        return std::dynamic_pointer_cast<T>(component);
    }

    template<IsComponent T>
    void ECSManager::removeComponent(const Entity &entity) {
        auto name = std::string(T::name());

        this->removeComponent(entity, std::move(name));
    }

    template<IsComponent T>
    bool ECSManager::hasComponent(const Entity &entity) const {
        return this->tryGetComponent<T>(entity).has_value();
    }

    template<IsComponent T>
    std::optional<std::shared_ptr<T>> ECSManager::tryGetComponent(const Entity &entity) const {
        auto name = std::string(T::name());
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

    template<IsComponent T>
    std::vector<Entity> ECSManager::queryComponents() const {
        auto name = std::string(T::name());
        return this->queryComponents(name);
    }

    template<IsSystem T>
    void ECSManager::registerSystem(bool activateImmediately) {
        auto name = std::string(T::name());

        if (this->_systems.find(name) != this->_systems.end()) {
            throw EngineError(fmt::format("System {} already registered", name));
        }

        this->_systems[name] = SystemEntry{
                .activateImmediately = activateImmediately,
                .enabled = false,
                .instance = new T(this->_resources)
        };
    }

    template<IsSystem T>
    void ECSManager::enableSystem() {
        auto name = std::string(T::name());
        this->enableSystem(std::move(name));
    }

    template<IsSystem T>
    void ECSManager::disableSystem() {
        auto name = std::string(T::name());
        this->disableSystem(std::move(name));
    }
}

#endif // PENROSE_ECS_ECS_MANAGER_INL
