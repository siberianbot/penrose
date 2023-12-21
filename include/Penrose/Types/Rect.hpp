#ifndef PENROSE_TYPES_RECT_HPP
#define PENROSE_TYPES_RECT_HPP

#include <cstdint>

namespace Penrose {

    /**
     * \brief Rectangle
     * \tparam T Types of rectangle components
     */
    template <typename T>
    struct PENROSE_API Rect {
        constexpr Rect(T x, T y, T w, T h)
            : x(x),
              y(y),
              w(w),
              h(h) {
            //
        }

        T x, y, w, h;

        friend auto operator<=>(const Rect &, const Rect &) = default;
    };

    /**
     * \brief Type alias for rectangle with integer components
     */
    using IntRect = Rect<std::int32_t>;

    /**
     * \brief Type alias for rectangle with float components
     */
    using FloatRect = Rect<float>;
}

#endif // PENROSE_TYPES_RECT_HPP
