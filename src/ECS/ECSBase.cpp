#include "ECSBase.hpp"

#include <limits>

namespace Penrose {

    constexpr inline int UINT32_DIGITS = std::numeric_limits<std::uint32_t>::digits;

    ComponentId makeComponentId(const Entity &entity, const ComponentType &componentType) {
        ComponentId id = 0;

        id |= entity;
        id |= static_cast<std::uint64_t>(componentType) << UINT32_DIGITS;

        return id;
    }

    std::tuple<Entity, ComponentType> deconstructComponentId(const ComponentId &componentId) {
        auto entity = static_cast<std::uint32_t>(componentId) & static_cast<std::uint32_t>(-1);
        auto componentType = static_cast<std::uint32_t>(componentId >> UINT32_DIGITS) & static_cast<std::uint32_t>(-1);

        return std::make_tuple(entity, componentType);
    }
}
