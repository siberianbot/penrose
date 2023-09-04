#ifndef PENROSE_COMMON_ENGINE_ERROR_INL
#define PENROSE_COMMON_ENGINE_ERROR_INL

#include <utility>

namespace Penrose {

    template<typename... Args>
    constexpr EngineError::EngineError(fmt::format_string<Args...> fmt, Args &&... args)
            : EngineError(fmt::format(std::forward<fmt::format_string<Args...>>(fmt), std::forward<Args>(args)...)) {
        //
    }
}

#endif // PENROSE_COMMON_ENGINE_ERROR_INL
