#ifndef PENROSE_UTILS_OPTIONAL_UTILS_HPP
#define PENROSE_UTILS_OPTIONAL_UTILS_HPP

#include <optional>

namespace Penrose {

    template<typename T, typename Callable>
    constexpr auto map(const std::optional<T> &opt, Callable callable) {
        using Result = std::invoke_result_t<Callable, T>;

        if (opt.has_value()) {
            return std::optional<Result>(callable(*opt));
        } else {
            return std::optional<Result>{};
        }
    }
}

#endif // PENROSE_UTILS_OPTIONAL_UTILS_HPP
