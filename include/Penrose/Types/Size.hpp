#ifndef PENROSE_TYPES_SIZE_HPP
#define PENROSE_TYPES_SIZE_HPP

#include <cstdint>
#include <tuple>

namespace Penrose {

    /**
     * \brief Tuple for two-dimensional size (width and height)
     */
    using Size = std::tuple<std::uint32_t, std::uint32_t>;
}

#endif // PENROSE_TYPES_SIZE_HPP
