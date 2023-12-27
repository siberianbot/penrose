#ifndef PENROSE_ECS_ENTITY_STORE_HPP
#define PENROSE_ECS_ENTITY_STORE_HPP

#include <map>
#include <memory>
#include <vector>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/Entity.hpp>

#include "src/Common/BitSet.hpp"

namespace Penrose {

    class EntityStore {
    public:
        static constexpr std::size_t DEFAULT_SIZE = 64 * 1024;

        using ComponentCollection = std::map<ComponentType, std::shared_ptr<ComponentPtr>>;

        EntityStore();

        void reset();

        [[nodiscard]] Entity acquire();
        void release(Entity entity);

        [[nodiscard]] bool acquired(Entity entity) const;
        [[nodiscard]] ComponentCollection &get(Entity entity);

        [[nodiscard]] std::size_t size() const;

    private:
        BitSet _usage;
        std::vector<ComponentCollection> _entities;

        void resize(std::size_t size);
    };
}

#endif // PENROSE_ECS_ENTITY_STORE_HPP
