#ifndef PENROSE_ECS_ENTITY_QUERY_HPP
#define PENROSE_ECS_ENTITY_QUERY_HPP

#include <list>
#include <memory>
#include <optional>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/Entity.hpp>
#include <Penrose/ECS/EntityEntry.hpp>
#include <Penrose/ECS/EntityIterator.hpp>

namespace Penrose {

    /**
     * \brief Entity query
     * \details Query acts as adapter over EntityIterator with additional filtering capabilities.
     */
    class PENROSE_API EntityQuery {
    public:
        explicit EntityQuery(std::unique_ptr<EntityIterator> &&iterator);

        EntityQuery(EntityQuery &&) = default;
        EntityQuery &operator=(EntityQuery &&) = default;

        EntityQuery(const EntityQuery &) = delete;
        EntityQuery &operator=(const EntityQuery &) = delete;

        /**
         * \brief Apply filtering by entity
         * \param entity Entity
         * \return This instance
         */
        [[nodiscard]] EntityQuery &entity(Entity entity);

        /**
         * \brief Apply filtering by component
         * \param componentType Type of component
         * \return This instance
         */
        [[nodiscard]] EntityQuery &component(ComponentType &&componentType);

        /**
         * \brief Apply filtering by component
         * \tparam T Type of component
         * \return This instance
         */
        template <typename T>
        requires std::is_base_of_v<Component<T>, T>
        [[nodiscard]] EntityQuery &component() {
            return this->component(T::type());
        }

        /**
         * \brief Try to move underlying iterator to next element and return the resulting element
         * \return Resulting element or nothing
         */
        [[nodiscard]] std::optional<EntityEntry> front() const;

        /**
         * \brief Collect all available entries
         * \return List of entries
         */
        [[nodiscard]] std::list<EntityEntry> collect() const;

    private:
        std::unique_ptr<EntityIterator> _iterator;
    };
}

#endif // PENROSE_ECS_ENTITY_QUERY_HPP
