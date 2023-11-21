#ifndef PENROSE_COMMON_ENGINE_ERROR_HPP
#define PENROSE_COMMON_ENGINE_ERROR_HPP

#include <exception>
#include <string>
#include <string_view>

#include <fmt/core.h>

namespace Penrose {

    class EngineError : public std::exception {
    private:
        std::string _msg;

    public:
        explicit EngineError(std::string_view msg) : _msg(msg) {
            //
        }

        template<typename ...Args>
        constexpr EngineError(fmt::format_string<Args...> fmt, Args &&...args);

        ~EngineError() noexcept override = default;

        [[nodiscard]] const char *what() const noexcept override {
            return this->_msg.c_str();
        }

        [[nodiscard]] static EngineError notImplemented() {
            return EngineError("Not implemented");
        }
    };
}

#include "EngineError.inl"

#endif // PENROSE_COMMON_ENGINE_ERROR_HPP
