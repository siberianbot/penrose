#ifndef PENROSE_CORE_LOG_INL
#define PENROSE_CORE_LOG_INL

#include <utility>

namespace Penrose {

    template<typename... Args>
    constexpr void Log::writeDebug(std::string_view tag, fmt::format_string<Args...> fmt, Args &&... args) {
        this->writeDebug(tag,
                         fmt::format(std::forward<fmt::format_string<Args...>>(fmt), std::forward<Args>(args)...));
    }

    template<typename... Args>
    constexpr void Log::writeInfo(std::string_view tag, fmt::format_string<Args...> fmt, Args &&... args) {
        this->writeInfo(tag,
                        fmt::format(std::forward<fmt::format_string<Args...>>(fmt), std::forward<Args>(args)...));
    }

    template<typename... Args>
    constexpr void Log::writeWarning(std::string_view tag, fmt::format_string<Args...> fmt, Args &&... args) {
        this->writeWarning(tag,
                           fmt::format(std::forward<fmt::format_string<Args...>>(fmt), std::forward<Args>(args)...));
    }

    template<typename... Args>
    constexpr void Log::writeError(std::string_view tag, fmt::format_string<Args...> fmt, Args &&... args) {
        this->writeError(tag,
                         fmt::format(std::forward<fmt::format_string<Args...>>(fmt), std::forward<Args>(args)...));
    }
}

#endif // PENROSE_CORE_LOG_INL
