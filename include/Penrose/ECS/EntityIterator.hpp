#ifndef PENROSE_ECS_ENTITY_ITERATOR_HPP
#define PENROSE_ECS_ENTITY_ITERATOR_HPP

#include <Penrose/ECS/EntityEntry.hpp>

namespace Penrose {

    /**
     * \brief Entity iterator interface
     * \details Such iterators allows to iterate through all entiites and their components in our ECS system. Iterators
     * locks EntityManager for any entity modification to prevent concurrent access and keep internal state consistent,
     * i.e. while rendering entities.
     */
    class PENROSE_API EntityIterator {
    public:
        virtual ~EntityIterator() = default;

        /**
         * \brief Move iterator to next element
         * \return Entry availability
         */
        [[nodiscard]] virtual bool move() = 0;

        /**
         * \brief Get entity entry with current element
         * \return Entity entry with current element
         */
        [[nodiscard]] virtual EntityEntry fetch() = 0;
    };
}

#endif // PENROSE_ECS_ENTITY_ITERATOR_HPP
