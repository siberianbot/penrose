#ifndef PENROSE_COMMON_LOG_HPP
#define PENROSE_COMMON_LOG_HPP

#include <string_view>

#include <fmt/core.h>

#include <Penrose/Api.hpp>
#include <Penrose/Common/LogLevel.hpp>
#include <Penrose/Common/LogSink.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    class PENROSE_API Log : public Resource<Log, ResourceGroup::Engine> {
    public:
        explicit Log(ResourceSet *resources);
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
        ResourceProxy<LogSink> _sinks;

        void write(LogLevel level, std::string_view tag, std::string_view msg);
    };
}

#include "Log.inl"

#endif // PENROSE_COMMON_LOG_HPP
