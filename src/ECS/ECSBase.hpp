#ifndef PENROSE_ECS_ECS_BASE_HPP
#define PENROSE_ECS_ECS_BASE_HPP

#include <cstdint>
#include <tuple>

namespace Penrose {

    using Entity = std::uint32_t;
    using ComponentType = std::uint32_t;
    using ComponentId = std::uint64_t;

    [[nodiscard]] ComponentId makeComponentId(const Entity &entity, const ComponentType &componentType);

    [[nodiscard]] std::tuple<Entity, ComponentType> deconstructComponentId(const ComponentId &componentId);

    class Component {
    public:
        virtual ~Component() = default;
    };
}

#endif // PENROSE_ECS_ECS_BASE_HPP
