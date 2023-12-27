#include "EntityStore.hpp"

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    EntityStore::EntityStore()
        : _usage(DEFAULT_SIZE),
          _entities(DEFAULT_SIZE) {
        //
    }

    void EntityStore::reset() {
        this->_usage.clear();
        this->_entities.clear();

        this->resize(DEFAULT_SIZE);
    }

    Entity EntityStore::acquire() {
        if (this->_usage.all()) {
            const auto size = this->_usage.size();
            this->resize(size + size / 2);
        }

        for (std::uint32_t idx = 0; idx < this->_usage.size(); ++idx) {
            if (this->_usage.at(idx)) {
                continue;
            }

            this->_usage.set(idx, true);

            return idx;
        }

        throw EngineError("Failed to acquire entity");
    }

    void EntityStore::release(const Entity entity) {
        if (!this->_usage.at(entity)) {
            throw EngineError("Entity {} is not acquired", entity);
        }

        this->_usage.set(entity, false);
        this->_entities.at(entity) = {};
    }

    bool EntityStore::acquired(const Entity entity) const {
        return this->_usage.at(entity);
    }

    EntityStore::ComponentCollection &EntityStore::get(const Entity entity) {
        if (!this->_usage.at(entity)) {
            throw EngineError("Entity {} is not acquired", entity);
        }

        return this->_entities.at(entity);
    }

    std::size_t EntityStore::size() const {
        return this->_usage.size();
    }

    void EntityStore::resize(const std::size_t size) {
        this->_usage.resize(size);
        this->_entities.resize(size, {});
    }
}
