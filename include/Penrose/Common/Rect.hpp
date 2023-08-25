#ifndef PENROSE_COMMON_RECT_HPP
#define PENROSE_COMMON_RECT_HPP

#include <cstdint>

namespace Penrose {

    template<typename T>
    struct Rect {
        Rect(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) {
            //
        }

        T x, y, w, h;
    };

    using IntRect = Rect<std::int32_t>;
    using FloatRect = Rect<float>;
}

#endif // PENROSE_COMMON_RECT_HPP
