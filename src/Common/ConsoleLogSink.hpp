#ifndef PENROSE_COMMON_CONSOLE_LOG_SINK_HPP
#define PENROSE_COMMON_CONSOLE_LOG_SINK_HPP

#include <Penrose/Common/LogSink.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ConsoleLogSink final: public Resource<ConsoleLogSink>,
                                public LogSink {
    public:
        ~ConsoleLogSink() override = default;

        void write(LogLevel level, std::string_view tag, std::string_view msg) override;
    };
}

#endif // PENROSE_COMMON_CONSOLE_LOG_SINK_HPP
