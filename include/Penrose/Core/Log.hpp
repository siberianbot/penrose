#ifndef PENROSE_CORE_LOG_HPP
#define PENROSE_CORE_LOG_HPP

#include <string_view>

#include <fmt/core.h>

#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    enum class LogLevel {
        Debug,
        Info,
        Warning,
        Error
    };

    class Log : public Resource {
    public:
        ~Log() override = default;

        void writeDebug(std::string_view tag, std::string_view msg);
        void writeInfo(std::string_view tag, std::string_view msg);
        void writeWarning(std::string_view tag, std::string_view msg);
        void writeError(std::string_view tag, std::string_view msg);

        template<typename ...Args>
        constexpr void writeDebug(std::string_view tag, fmt::format_string<Args...> fmt, Args &&... args);

        template<typename ...Args>
        constexpr void writeInfo(std::string_view tag, fmt::format_string<Args...> fmt, Args &&... args);

        template<typename ...Args>
        constexpr void writeWarning(std::string_view tag, fmt::format_string<Args...> fmt, Args &&... args);

        template<typename ...Args>
        constexpr void writeError(std::string_view tag, fmt::format_string<Args...> fmt, Args &&... args);

    private:
        void write(LogLevel level, std::string_view tag, std::string_view msg);
    };
}

#include "Log.inl"

#endif // PENROSE_CORE_LOG_HPP
