#ifndef PENROSE_UI_VALUE_HPP
#define PENROSE_UI_VALUE_HPP

#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/UI/ValueType.hpp>

namespace Penrose {

    enum class WidgetValueSource {
        Constant,
        Binding
    };

    template<typename T, ValueType Type>
    class StrongTypedWidgetValue {
    public:
        template<typename V>
        requires std::is_convertible_v<V, T>
        explicit StrongTypedWidgetValue(V constant)
                : _source(WidgetValueSource::Constant),
                  _constant(constant) {
            //
        }

        StrongTypedWidgetValue(WidgetValueSource source, T constant, std::string &&binding)
                : _source(source),
                  _constant(constant),
                  _binding(binding) {
            //
        }

        [[nodiscard]] ValueType getType() const { return Type; }

        [[nodiscard]] WidgetValueSource getSource() const { return this->_source; }

        [[nodiscard]] const T &getConstant() const {
            if (this->_source != WidgetValueSource::Constant) {
                throw EngineError("Value is not a constant");
            }

            return this->_constant;
        }

        [[nodiscard]] const std::string &getBinding() const {
            if (this->_type != WidgetValueSource::Binding) {
                throw EngineError("Value is not a binding");
            }

            return this->_binding;
        }

        static StrongTypedWidgetValue<T, Type> parse(std::string_view &&) {
            throw EngineError("Value parsing is not supported");
        }

    private:
        WidgetValueSource _source;
        T _constant;
        std::string _binding;
    };

    using BooleanWidgetValue = StrongTypedWidgetValue<bool, ValueType::Boolean>;
    using IntegerWidgetValue = StrongTypedWidgetValue<int, ValueType::Integer>;
    using FloatWidgetValue = StrongTypedWidgetValue<float, ValueType::Float>;
    using StringWidgetValue = StrongTypedWidgetValue<std::string, ValueType::String>;

    template<ValueType Type>
    class BindOnlyWidgetValue {
    public:
        explicit BindOnlyWidgetValue(std::string &&binding)
                : _binding(binding) {
            //
        }

        [[nodiscard]] ValueType getType() const { return Type; }

        [[nodiscard]] const std::string &getBinding() { return this->_binding; }

        static BindOnlyWidgetValue<Type> parse(std::string_view &&value) {
            throw EngineError("Value parsing is not supported");
        }

    private:
        std::string _binding;
    };

    using ActionWidgetValue = BindOnlyWidgetValue<ValueType::Action>;
    using ObjectWidgetValue = BindOnlyWidgetValue<ValueType::Object>;
    using ListWidgetValue = BindOnlyWidgetValue<ValueType::List>;
}

#endif // PENROSE_UI_VALUE_HPP
