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

    template<typename A, typename B = A>
    requires std::is_convertible_v<B, A>
    constexpr inline A max(const A &a, const B &b) {
        return a < b ? b : a;
    }

    template<typename A, typename B = A, typename ...Args>
    requires std::is_convertible_v<B, A>
    constexpr inline A max(const A &a, const B &b, const Args &...args) {
        return max(max(a, b), args...);
    }
}

#endif // PENROSE_MATH_NUMERIC_FUNCS_HPP
