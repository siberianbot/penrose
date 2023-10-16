#ifndef PENROSE_UTILS_HASH_UTILS_HPP
#define PENROSE_UTILS_HASH_UTILS_HPP

#include <initializer_list>
#include <utility>

#include <Penrose/Utils/CollectionUtils.hpp>

namespace Penrose {

    template<IsCollection T>
    std::size_t hashOf(const T &values) {
        auto h = std::hash<std::size_t>{}(values.size());

        auto idx = 0;
        for (const auto &value: values) {
            h ^= std::hash<std::remove_cvref_t<decltype(value)>>{}(value) << idx;

            idx++;
        }

        return h;
    }
}

#endif // PENROSE_UTILS_HASH_UTILS_HPP
