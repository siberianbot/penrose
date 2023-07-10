#ifndef PENROSE_COMMON_ENGINE_ERROR_HPP
#define PENROSE_COMMON_ENGINE_ERROR_HPP

#include <exception>
#include <string>
#include <utility>

namespace Penrose {

    class EngineError : public std::exception {
    private:
        std::string _msg;

    public:
        explicit EngineError(std::string msg) : _msg(std::move(msg)) {
            //
        }

        ~EngineError() noexcept override = default;

        [[nodiscard]] const char *what() const noexcept override {
            return this->_msg.c_str();
        }
    };
}

#endif // PENROSE_COMMON_ENGINE_ERROR_HPP
