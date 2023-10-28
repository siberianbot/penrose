#ifndef PENROSE_UI_VALUE_HPP
#define PENROSE_UI_VALUE_HPP

#include <string>
#include <string_view>
#include <type_traits>

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    enum class ValueType {
        Constant,
        Binding
    };

    template<typename T>
    class Value {
    public:
        template<typename V>
        requires std::is_convertible_v<V, T>
        Value(V constant) // NOLINT(google-explicit-constructor)
                : _type(ValueType::Constant),
                  _constant(constant) {
            //
        }

        Value(ValueType type, T constant, std::string &&binding)
                : _type(type),
                  _constant(constant),
                  _binding(binding) {
            //
        }

        [[nodiscard]] const T &getConstant() const {
            if (this->_type != ValueType::Constant) {
                throw EngineError("Value is not a constant");
            }

            return this->_constant;
        }

        [[nodiscard]] const std::string &getBinding() {
            if (this->_type != ValueType::Binding) {
                throw EngineError("Value is not a binding");
            }

            return this->_binding;
        }

        static Value<T> parse(std::string_view &&) {
            throw EngineError("Value parsing is not supported");
        }

    private:
        ValueType _type;
        T _constant;
        std::string _binding;
    };

    using BooleanValue = Value<bool>;
    using IntegerValue = Value<int>;
    using FloatValue = Value<float>;
    using StringValue = Value<std::string>;
}

#endif // PENROSE_UI_VALUE_HPP
