#ifndef PENROSE_ECS_ENTITY_FILTER_ITERATOR_HPP
#define PENROSE_ECS_ENTITY_FILTER_ITERATOR_HPP

#include <memory>
#include <set>

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/ECS/EntityIterator.hpp>

namespace Penrose {

    class EntityFilterIterator final: public EntityIterator {
    public:
        EntityFilterIterator(std::unique_ptr<EntityIterator> &&next, std::set<Entity> &&entities);
        ~EntityFilterIterator() override = default;

        [[nodiscard]] bool move() override;
        [[nodiscard]] EntityEntry fetch() override;

        void filter(Entity entity);

    private:
        std::unique_ptr<EntityIterator> _next;
        std::set<Entity> _entities;
    };
}

#endif // PENROSE_ECS_ENTITY_FILTER_ITERATOR_HPP
