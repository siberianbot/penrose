#ifndef PENROSE_ECS_COMPONENT_FILTER_ITERATOR_HPP
#define PENROSE_ECS_COMPONENT_FILTER_ITERATOR_HPP

#include <memory>
#include <set>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/EntityIterator.hpp>

namespace Penrose {

    class ComponentFilterIterator final: public EntityIterator {
    public:
        ComponentFilterIterator(std::unique_ptr<EntityIterator> &&next, std::set<ComponentType> &&components);
        ~ComponentFilterIterator() override = default;

        [[nodiscard]] bool move() override;
        [[nodiscard]] EntityEntry fetch() override;

        void filter(ComponentType &&componentType);

    private:
        std::unique_ptr<EntityIterator> _next;
        std::set<ComponentType> _components;
    };
}

#endif // PENROSE_ECS_COMPONENT_FILTER_ITERATOR_HPP
