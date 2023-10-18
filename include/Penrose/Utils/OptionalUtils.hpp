#ifndef PENROSE_UTILS_OPTIONAL_UTILS_HPP
#define PENROSE_UTILS_OPTIONAL_UTILS_HPP

#include <map>
#include <optional>

namespace Penrose {

    template<typename T>
    struct OptionalType;

    template<typename T>
    struct OptionalType<std::optional<T>> {
        using Type = T;
    };

    template<typename T, typename E>
    constexpr auto orElseThrow(const std::optional<T> &opt, E ex) {
        if (opt.has_value()) {
            return *opt;
        }

        throw ex;
    }

    template<typename T, typename Callable, typename Result = std::invoke_result_t<Callable, T>>
    constexpr std::optional<Result> map(const std::optional<T> &opt, Callable callable) {
        if (opt.has_value()) {
            return std::optional<Result>(callable(*opt));
        } else {
            return std::optional<Result>{};
        }
    }

    template<typename T, typename Callable, typename Result = OptionalType<std::invoke_result_t<Callable, T>>::Type>
    constexpr std::optional<Result> flatMap(const std::optional<T> &opt, Callable callable) {
        if (opt.has_value()) {
            return callable(*opt);
        } else {
            return std::optional<Result>{};
        }
    }

    template<typename K, typename V>
    constexpr std::optional<V> tryGet(const std::map<K, V> &map, const K &key) {
        auto it = map.find(key);

        if (it == map.end()) {
            return std::nullopt;
        }

        return it->second;
    }

    template<typename K, typename V>
    constexpr std::optional<const V *> tryGetPointer(const std::map<K, V> &map, const K &key) {
        auto it = map.find(key);

        if (it == map.end()) {
            return std::nullopt;
        }

        return &it->second;
    }

    template<typename K, typename V>
    constexpr std::optional<V *> tryGetPointer(std::map<K, V> &map, const K &key) {
        auto it = map.find(key);

        if (it == map.end()) {
            return std::nullopt;
        }

        return &it->second;
    }
}

#endif // PENROSE_UTILS_OPTIONAL_UTILS_HPP
