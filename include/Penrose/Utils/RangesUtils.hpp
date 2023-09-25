#ifndef PENROSE_UTILS_RANGES_UTILS_HPP
#define PENROSE_UTILS_RANGES_UTILS_HPP

#include <map>
#include <ranges>
#include <tuple>

namespace Penrose {

    template<typename K, typename V, std::ranges::input_range R>
    constexpr std::map<K, V> toMap(R &&range) {
        std::map<K, V> result;

        for (const auto &[key, value]: range) {
            result.emplace(key, value);
        }

        return result;
    }
}

#endif // PENROSE_UTILS_RANGES_UTILS_HPP
