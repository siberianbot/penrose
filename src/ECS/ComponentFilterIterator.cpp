#include "ComponentFilterIterator.hpp"

namespace Penrose {

    ComponentFilterIterator::ComponentFilterIterator(
        std::unique_ptr<EntityIterator> &&next, std::set<ComponentType> &&components
    )
        : _next(std::forward<decltype(next)>(next)),
          _components(std::forward<decltype(components)>(components)) {
        //
    }

    bool ComponentFilterIterator::move() {
        do {
            if (!this->_next->move()) {
                return false;
            }
        } while (!this->_components.contains(this->_next->fetch().componentType));

        return true;
    }

    EntityEntry ComponentFilterIterator::fetch() {
        return this->_next->fetch();
    }

    void ComponentFilterIterator::filter(ComponentType &&componentType) {
        this->_components.insert(std::forward<decltype(componentType)>(componentType));
    }
}
