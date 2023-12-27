#include "EntityFilterIterator.hpp"

namespace Penrose {

    EntityFilterIterator::EntityFilterIterator(std::unique_ptr<EntityIterator> &&next, std::set<Entity> &&entities)
        : _next(std::forward<decltype(next)>(next)),
          _entities(std::forward<decltype(entities)>(entities)) {
        //
    }

    bool EntityFilterIterator::move() {
        do {
            if (!this->_next->move()) {
                return false;
            }
        } while (!this->_entities.contains(this->_next->fetch().entity));

        return true;
    }

    EntityEntry EntityFilterIterator::fetch() {
        return this->_next->fetch();
    }

    void EntityFilterIterator::filter(const Entity entity) {
        this->_entities.insert(entity);
    }
}
