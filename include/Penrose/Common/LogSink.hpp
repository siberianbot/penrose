#ifndef PENROSE_COMMON_LOG_SINK_HPP
#define PENROSE_COMMON_LOG_SINK_HPP

#include <string_view>

#include <Penrose/Common/LogLevel.hpp>

namespace Penrose {

    class LogSink {
    public:
        virtual ~LogSink() = default;

        virtual void write(LogLevel level, std::string_view tag, std::string_view msg) = 0;
    };
}

#endif // PENROSE_COMMON_LOG_SINK_HPP
