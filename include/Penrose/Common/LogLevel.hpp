#ifndef PENROSE_COMMON_LOG_LEVEL_HPP
#define PENROSE_COMMON_LOG_LEVEL_HPP

#include <string_view>

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    enum class LogLevel {
        Debug,
        Info,
        Warning,
        Error
    };

    constexpr std::string_view toString(LogLevel level) {
        switch (level) {
            case LogLevel::Debug:
                return "Debug";

            case LogLevel::Info:
                return "Info";

            case LogLevel::Warning:
                return "Warning";

            case LogLevel::Error:
                return "Error";

            default:
                throw EngineError("Log level is not supported");
        }
    }
}

#endif // PENROSE_COMMON_LOG_LEVEL_HPP
