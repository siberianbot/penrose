#ifndef PENROSE_COMMON_LOG_HPP
#define PENROSE_COMMON_LOG_HPP

#include <string_view>
#include <typeindex>

#include <fmt/core.h>

#include <Penrose/Common/LogLevel.hpp>
#include <Penrose/Common/LogSink.hpp>

namespace Penrose {

    /**
     * \brief Log system interface
     * \details This interface provides methods of log system.
     */
    class PENROSE_API Log {
    public:
        virtual ~Log() = default;

        /**
         * \brief Write log message
         * \param level Message level
         * \param tag Message tag
         * \param msg Log message
         */
        virtual void write(LogLevel level, std::string_view tag, std::string_view msg) = 0;

        /**
         * \brief Writes debug message into log
         * \param tag Message tag (i.e. class name)
         * \param msg Log message
         */
        constexpr void writeDebug(const std::string_view tag, const std::string_view msg) {
            this->write(LogLevel::Debug, tag, msg);
        }

        /**
         * \brief Writes debug message into log
         * \tparam Args Log message format string argument types
         * \param tag Message tag (i.e. class name)
         * \param fmt Format string
         * \param args Format string arguments
         */
        template <typename... Args>
        constexpr void writeDebug(const std::string_view tag, fmt::format_string<Args...> fmt, Args &&...args) {
            this->write(LogLevel::Debug, tag, fmt::format(std::move(fmt), std::forward<Args>(args)...));
        }

        /**
         * \brief Writes informational message into log
         * \param tag Message tag (i.e. class name)
         * \param msg Log message
         */
        constexpr void writeInfo(const std::string_view tag, const std::string_view msg) {
            this->write(LogLevel::Info, tag, msg);
        }

        /**
         * \brief Writes informational message into log
         * \tparam Args Log message format string argument types
         * \param tag Message tag (i.e. class name)
         * \param fmt Format string
         * \param args Format string arguments
         */
        template <typename... Args>
        constexpr void writeInfo(const std::string_view tag, fmt::format_string<Args...> fmt, Args &&...args) {
            this->write(LogLevel::Info, tag, fmt::format(std::move(fmt), std::forward<Args>(args)...));
        }

        /**
         * \brief Writes warning message into log
         * \param tag Message tag (i.e. class name)
         * \param msg Log message
         */
        constexpr void writeWarning(const std::string_view tag, const std::string_view msg) {
            this->write(LogLevel::Warning, tag, msg);
        }

        /**
         * \brief Writes warning message into log
         * \tparam Args Log message format string argument types
         * \param tag Message tag (i.e. class name)
         * \param fmt Format string
         * \param args Format string arguments
         */
        template <typename... Args>
        constexpr void writeWarning(const std::string_view tag, fmt::format_string<Args...> fmt, Args &&...args) {
            this->write(LogLevel::Warning, tag, fmt::format(std::move(fmt), std::forward<Args>(args)...));
        }

        /**
         * \brief Writes error message into log
         * \param tag Message tag (i.e. class name)
         * \param msg Log message
         */
        constexpr void writeError(const std::string_view tag, const std::string_view msg) {
            this->write(LogLevel::Error, tag, msg);
        }

        /**
         * \brief Writes error message into log
         * \tparam Args Log message format string argument types
         * \param tag Message tag (i.e. class name)
         * \param fmt Format string
         * \param args Format string arguments
         */
        template <typename... Args>
        constexpr void writeError(const std::string_view tag, fmt::format_string<Args...> fmt, Args &&...args) {
            this->write(LogLevel::Error, tag, fmt::format(std::move(fmt), std::forward<Args>(args)...));
        }

        /**
         * \brief Add log sink
         * \param index Type index of log sink
         */
        virtual void addSink(std::type_index &&index) = 0;

        /**
         * \brief Add log sink
         * \tparam T Type of log sink
         */
        template <typename T>
        requires std::is_base_of_v<LogSink, T>
        void addSink() {
            this->addSink(std::type_index(typeid(T)));
        }
    };
}

#endif // PENROSE_COMMON_LOG_HPP
