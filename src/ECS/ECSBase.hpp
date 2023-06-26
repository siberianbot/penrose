#ifndef PENROSE_ECS_ECS_BASE_HPP
#define PENROSE_ECS_ECS_BASE_HPP

#include <cstdint>
#include <string_view>

namespace Penrose {

    using Entity = std::uint32_t;
    using ComponentId = std::uint64_t;
    using ComponentName = std::string_view;
}

#endif // PENROSE_ECS_ECS_BASE_HPP
