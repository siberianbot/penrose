#ifndef PENROSE_COMMON_LOG_SINK_HPP
#define PENROSE_COMMON_LOG_SINK_HPP

#include <string_view>

#include <Penrose/Common/LogLevel.hpp>

namespace Penrose {

    /**
     * \brief Log system sink
     */
    class PENROSE_API LogSink {
    public:
        virtual ~LogSink() = default;

        /**
         * \brief Write message into sink
         * \param level Message level
         * \param tag Message tag (i.e. class name)
         * \param msg Log message
         */
        virtual void write(LogLevel level, std::string_view tag, std::string_view msg) = 0;
    };
}

#endif // PENROSE_COMMON_LOG_SINK_HPP
