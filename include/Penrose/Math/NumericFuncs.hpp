#ifndef PENROSE_MATH_NUMERIC_FUNCS_HPP
#define PENROSE_MATH_NUMERIC_FUNCS_HPP

#include <type_traits>

namespace Penrose {

    template<typename V, typename Min = V, typename Max = V>
    requires std::is_convertible_v<Min, V> && std::is_convertible_v<Max, V>
    constexpr inline V cycle(const V &value, const Min &min, const Max &max) {

        auto dist = max - min;

        if (value < min) {
            return value + dist;
        }

        if (value > max) {
            return value - dist;
        }

        return value;
    }
}

#endif // PENROSE_MATH_NUMERIC_FUNCS_HPP
