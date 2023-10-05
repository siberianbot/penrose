#ifndef PENROSE_UTILS_HASH_UTILS_HPP
#define PENROSE_UTILS_HASH_UTILS_HPP

#include <initializer_list>
#include <utility>

namespace Penrose {

    template<typename T>
    concept IsCollection = requires(const T t) {
        requires std::forward_iterator<typename T::const_iterator>;
        { t.begin() } -> std::same_as<typename T::const_iterator>;
        { t.end() } -> std::same_as<typename T::const_iterator>;
        { t.size() } -> std::same_as<typename T::size_type>;
    };

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
