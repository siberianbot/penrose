#include <Penrose/ECS/EntityQuery.hpp>

#include "src/ECS/ComponentFilterIterator.hpp"
#include "src/ECS/EntityFilterIterator.hpp"

namespace Penrose {

    EntityQuery::EntityQuery(std::unique_ptr<EntityIterator> &&iterator)
        : _iterator(std::forward<decltype(iterator)>(iterator)) {
        //
    }

    EntityQuery &EntityQuery::entity(const Entity entity) {
        const auto asEntityFilter = dynamic_cast<EntityFilterIterator *>(this->_iterator.get());

        if (asEntityFilter != nullptr) {
            asEntityFilter->filter(entity);
        } else {
            this->_iterator = std::make_unique<EntityFilterIterator>(std::move(this->_iterator), std::set {entity});
        }

        return *this;
    }

    EntityQuery &EntityQuery::component(ComponentType &&componentType) {
        const auto asComponentFilter = dynamic_cast<ComponentFilterIterator *>(this->_iterator.get());

        if (asComponentFilter != nullptr) {
            asComponentFilter->filter(std::forward<decltype(componentType)>(componentType));
        } else {
            this->_iterator = std::make_unique<ComponentFilterIterator>(
                std::move(this->_iterator), std::set {std::forward<decltype(componentType)>(componentType)}
            );
        }

        return *this;
    }

    std::optional<EntityEntry> EntityQuery::front() const {
        if (!this->_iterator->move()) {
            return std::nullopt;
        }

        return this->_iterator->fetch();
    }

    std::list<EntityEntry> EntityQuery::collect() const {
        std::list<EntityEntry> entries;

        while (this->_iterator->move()) {
            entries.emplace_back(this->_iterator->fetch());
        }

        return entries;
    }
}
