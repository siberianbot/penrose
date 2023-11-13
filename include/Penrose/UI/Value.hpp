#ifndef PENROSE_UI_VALUE_HPP
#define PENROSE_UI_VALUE_HPP

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/UI/ValueType.hpp>

namespace Penrose {

    enum class ValueSource {
        Constant,
        Binding
    };

    template<typename T, ValueType Type>
    class StrongTypedValue {
    public:
        template<typename V>
        requires std::is_convertible_v<V, T>
        explicit StrongTypedValue(V constant)
                : _source(ValueSource::Constant),
                  _constant(constant) {
            //
        }

        StrongTypedValue(ValueSource source, T constant, std::string &&binding)
                : _source(source),
                  _constant(constant),
                  _binding(binding) {
            //
        }

        [[nodiscard]] ValueType getType() const { return Type; }

        [[nodiscard]] ValueSource getSource() const { return this->_source; }

        [[nodiscard]] const T &getConstant() const {
            if (this->_source != ValueSource::Constant) {
                throw EngineError("Value is not a constant");
            }

            return this->_constant;
        }

        [[nodiscard]] const std::string &getBinding() const {
            if (this->_source != ValueSource::Binding) {
                throw EngineError("Value is not a binding");
            }

            return this->_binding;
        }

        static StrongTypedValue<T, Type> parse(std::string_view &&) {
            throw EngineError("Value parsing is not supported");
        }

    private:
        ValueSource _source;
        T _constant;
        std::string _binding;
    };

    using BooleanValue = StrongTypedValue<bool, ValueType::Boolean>;
    using IntegerValue = StrongTypedValue<int, ValueType::Integer>;
    using FloatValue = StrongTypedValue<float, ValueType::Float>;
    using StringValue = StrongTypedValue<std::string, ValueType::String>;

    template<>
    BooleanValue BooleanValue::parse(std::string_view &&);

    template<>
    IntegerValue IntegerValue::parse(std::string_view &&);

    template<>
    FloatValue FloatValue::parse(std::string_view &&);

    template<>
    StringValue StringValue::parse(std::string_view &&);

    template<ValueType Type>
    class BindOnlyValue {
    public:
        explicit BindOnlyValue()
                : _binding(std::nullopt) {
            //
        }

        explicit BindOnlyValue(std::string &&binding)
                : _binding(binding) {
            //
        }

        [[nodiscard]] ValueType getType() const { return Type; }

        [[nodiscard]] const std::optional<std::string> &getBinding() const { return this->_binding; }

        static BindOnlyValue<Type> parse(std::string_view &&) {
            throw EngineError("Value parsing is not supported");
        }

    private:
        std::optional<std::string> _binding;
    };

    using ActionValue = BindOnlyValue<ValueType::Action>;
    using ObjectValue = BindOnlyValue<ValueType::Object>;
    using ListValue = BindOnlyValue<ValueType::List>;

    template<>
    ActionValue ActionValue::parse(std::string_view &&);

    template<>
    ObjectValue ObjectValue::parse(std::string_view &&);

    template<>
    ListValue ListValue::parse(std::string_view &&);
}

#endif // PENROSE_UI_VALUE_HPP
