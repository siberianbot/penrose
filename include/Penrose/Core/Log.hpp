#ifndef PENROSE_CORE_LOG_HPP
#define PENROSE_CORE_LOG_HPP

#include <string_view>

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

        void writeDebug(std::string_view src, std::string_view msg) const;
        void writeInfo(std::string_view src, std::string_view msg) const;
        void writeWarning(std::string_view src, std::string_view msg) const;
        void writeError(std::string_view src, std::string_view msg) const;

    private:
        void write(LogLevel level, std::string_view src, std::string_view msg) const;
    };
}

#endif // PENROSE_CORE_LOG_HPP
