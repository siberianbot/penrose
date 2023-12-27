#ifndef PENROSE_ECS_ENTITY_MANAGER_HPP
#define PENROSE_ECS_ENTITY_MANAGER_HPP

#include <memory>
#include <optional>
#include <typeindex>

#include <Penrose/Common/Params.hpp>
#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/Entity.hpp>
#include <Penrose/ECS/EntityArchetype.hpp>
#include <Penrose/ECS/EntityIterator.hpp>
#include <Penrose/ECS/EntityQuery.hpp>

namespace Penrose {

    /**
     * \brief Entity manager interface
     * \details This interface provides methods for working with entities and their components.
     */
    class PENROSE_API EntityManager {
    public:
        virtual ~EntityManager() = default;

        /**
         * \brief Create entity without components
         * \return New entity
         */
        [[nodiscard]] virtual Entity createEntity() = 0;

        /**
         * \brief Create entity using entity archetype
         * \param archetype Name of entity archetype
         * \param params Archetype construction parameters
         * \return New entity
         */
        [[nodiscard]] virtual Entity createEntity(std::string_view &&archetype, const Params &params) = 0;

        /**
         * \brief Destroy entity and all its components
         * \param entity Entity
         */
        virtual void destroyEntity(Entity entity) = 0;

        /**
         * \brief Add component to entity
         * \param entity Entity
         * \param component Component
         */
        virtual void addComponent(Entity entity, std::shared_ptr<ComponentPtr> &&component) = 0;

        /**
         * \brief Construct component through default constructor and add to entity
         * \tparam T Type of component
         * \param entity Entity
         */
        template <typename T>
        requires std::is_base_of_v<Component<T>, T> && std::is_default_constructible_v<T>
        void addComponent(const Entity entity) {
            this->addComponent(entity, std::make_shared<T>());
        }

        /**
         * \brief Construct component through copy constructor and add to entity
         * \tparam T Type of component
         * \param entity Entity
         * \param component Component
         */
        template <typename T>
        requires std::is_base_of_v<Component<T>, T> && std::is_move_constructible_v<T>
        void addComponent(const Entity entity, T &&component) {
            this->addComponent(entity, std::make_shared<T>(std::forward<decltype(component)>(component)));
        }

        /**
         * \brief Remove component from entity
         * \param entity Entity
         * \param componentType Type of component
         */
        virtual void removeComponent(Entity entity, ComponentType &&componentType) = 0;

        /**
         * \brief Remove component from entity
         * \tparam T Type of component
         * \param entity Entity
         */
        template <typename T>
        requires std::is_base_of_v<Component<T>, T>
        void removeComponent(const Entity entity) {
            this->removeComponent(entity, T::type());
        }

        /**
         * \brief Try get instance of component
         * \param entity Entity
         * \param componentType Type of component
         * \return Component instance or nothing
         */
        [[nodiscard]] virtual std::optional<std::shared_ptr<ComponentPtr>> tryGetComponent(
            Entity entity, ComponentType &&componentType
        ) = 0;

        /**
         * \brief Try get instance of component
         * \tparam T Type of component
         * \param entity Entity
         * \return Component instance or nothing
         */
        template <typename T>
        requires std::is_base_of_v<Component<T>, T>
        [[nodiscard]] std::optional<std::shared_ptr<T>> tryGetComponent(const Entity entity) {
            const auto component = this->tryGetComponent(entity, T::type());

            if (!component.has_value()) {
                return std::nullopt;
            }

            return std::dynamic_pointer_cast<T>(*component);
        }

        /**
         * \brief Get instance of component
         * \param entity Entity
         * \param componentType Type of component
         * \return Component instance
         */
        [[nodiscard]] virtual std::shared_ptr<ComponentPtr> getComponent(
            Entity entity, ComponentType &&componentType
        ) = 0;

        /**
         * \brief Get instance of component
         * \tparam T Type of component
         * \param entity Entity
         * \return Component instance
         */
        template <typename T>
        requires std::is_base_of_v<Component<T>, T>
        [[nodiscard]] std::shared_ptr<T> getComponent(const Entity entity) {
            return std::dynamic_pointer_cast<T>(this->getComponent(entity, T::type()));
        }

        /**
         * \brief Check for component presence for target entity
         * \param entity Entity
         * \param componentType Type of component
         * \return Component presence
         */
        [[nodiscard]] bool hasComponent(const Entity entity, std::type_index &&componentType) {
            return this->tryGetComponent(entity, std::forward<decltype(componentType)>(componentType)).has_value();
        }

        /**
         * \brief Check for component presence for target entity
         * \tparam T Type of component
         * \param entity Entity
         * \return Component presence
         */
        template <typename T>
        requires std::is_base_of_v<Component<T>, T>
        [[nodiscard]] bool hasComponent(const Entity entity) {
            return this->hasComponent(entity, T::type());
        }

        /**
         * \brief Get iterator over all available entities
         * \return Iterator over all available entities, wrapped in std::unique_ptr
         */
        [[nodiscard]] virtual std::unique_ptr<EntityIterator> iterate() = 0;

        /**
         * \brief Get new instance of entity query
         * \return Instance of entity query
         */
        [[nodiscard]] virtual EntityQuery query() = 0;

        /**
         * \brief Add entity archetype
         * \param type Type of entity archetype
         */
        virtual void addArchetype(std::type_index &&type) = 0;

        /**
         * \brief Add entity archetype
         * \tparam T Type of entity archetype
         */
        template <typename T>
        requires std::is_base_of_v<EntityArchetype, T>
        void addArchetype() {
            this->addArchetype(typeid(T));
        }
    };
}

#endif // PENROSE_ECS_ENTITY_MANAGER_HPP
