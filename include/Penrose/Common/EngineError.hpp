#ifndef PENROSE_COMMON_ENGINE_ERROR_HPP
#define PENROSE_COMMON_ENGINE_ERROR_HPP

#include <exception>
#include <string>

#include <fmt/core.h>

namespace Penrose {

    /**
     * \brief Exception for any engine error
     */
    class PENROSE_API EngineError final: public std::exception {
    public:
        /**
         * \brief Construct exception with error message
         * \param msg Message
         */
        explicit EngineError(std::string_view &&msg)
            : _msg(std::forward<decltype(msg)>(msg)) {
            //
        }

        /**
         * \brief Format string for message and construct exception
         * \tparam Args Format string argument types
         * \param fmt Format string
         * \param args Format string arguments
         */
        template <typename... Args>
        explicit constexpr EngineError(fmt::format_string<Args...> &&fmt, Args &&...args)
            : EngineError(fmt::format(std::forward<decltype(fmt)>(fmt), std::forward<Args>(args)...)) {
            //
        }

        ~EngineError() noexcept override = default;

        /**
         * \brief Get error message
         * \return Message
         */
        [[nodiscard]] const char *what() const noexcept override { return this->_msg.c_str(); }

        /**
         * \brief Engine error for not implemented functionality
         * \return Error with message "Not implemented"
         */
        [[nodiscard]] static EngineError notImplemented() { return EngineError("Not implemented"); }

    private:
        std::string _msg;
    };
}

#endif // PENROSE_COMMON_ENGINE_ERROR_HPP
