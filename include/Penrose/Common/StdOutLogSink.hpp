#ifndef PENROSE_COMMON_STD_OUT_LOG_SINK_HPP
#define PENROSE_COMMON_STD_OUT_LOG_SINK_HPP

#include <Penrose/Common/LogSink.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class StdOutLogSink : public Resource<StdOutLogSink, ResourceGroup::Engine>,
                          public LogSink {
    public:
        ~StdOutLogSink() override = default;

        void write(LogLevel level, std::string_view tag, std::string_view msg) override;
    };
}

#endif // PENROSE_COMMON_STD_OUT_LOG_SINK_HPP
