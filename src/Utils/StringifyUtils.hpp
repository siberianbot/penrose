#ifndef PENROSE_UTILS_STRINGIFY_UTILS_HPP
#define PENROSE_UTILS_STRINGIFY_UTILS_HPP

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    constexpr std::string_view toString(const LogLevel level) {
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

#endif // PENROSE_UTILS_STRINGIFY_UTILS_HPP
