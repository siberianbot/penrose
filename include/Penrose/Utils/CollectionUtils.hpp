#ifndef PENROSE_UTILS_COLLECTION_UTILS_HPP
#define PENROSE_UTILS_COLLECTION_UTILS_HPP

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

    template<IsCollection T, typename Mapping>
    constexpr bool contains(const T &collection, const auto &value, Mapping mapping) {
        for (const auto &item: collection) {
            if (mapping(item) == value) {
                return true;
            }
        }

        return false;
    }

    template<IsCollection T>
    constexpr bool contains(const T &collection, const auto &value) {
        for (const auto &item: collection) {
            if (item == value) {
                return true;
            }
        }

        return false;
    }
}

#endif // PENROSE_UTILS_COLLECTION_UTILS_HPP
