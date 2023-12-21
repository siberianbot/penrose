#ifndef PENROSE_UTILS_RANGE_UTILS_HPP
#define PENROSE_UTILS_RANGE_UTILS_HPP

#include <ranges>
#include <vector>

namespace Penrose {

    template <std::ranges::range R, typename T = std::decay_t<std::ranges::range_value_t<R>>>
    [[nodiscard]] constexpr std::vector<T> toVector(R &&r) {
        return std::vector<T>(r.begin(), r.end());
    }
}

#endif // PENROSE_UTILS_RANGE_UTILS_HPP
