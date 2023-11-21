#ifndef PENROSE_UI_PROPERTY_HPP
#define PENROSE_UI_PROPERTY_HPP

#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/UI/ValueType.hpp>

namespace Penrose {

    enum class PropertyType {
        Constant,
        Binding
    };

    template <ValueType Type, typename T>
    class StrongTypedProperty {
    public:
        template <typename V>
        requires std::is_convertible_v<V, T>
        explicit StrongTypedProperty(V constant)
            : _propertyType(PropertyType::Constant),
              _constant(constant) {
            //
        }

        StrongTypedProperty(const PropertyType propertyType, T constant, std::string &&binding)
            : _propertyType(propertyType),
              _constant(constant),
              _binding(binding) {
            //
        }

        [[nodiscard]] ValueType getValueType() const { return Type; }

        [[nodiscard]] PropertyType getPropertyType() const { return this->_propertyType; }

        [[nodiscard]] const T &getConstant() const {
            if (this->_propertyType != PropertyType::Constant) {
                throw EngineError("Property does not contain constant value");
            }

            return this->_constant;
        }

        [[nodiscard]] const std::string &getBinding() const {
            if (this->_propertyType != PropertyType::Binding) {
                throw EngineError("Property does not contain binding path");
            }

            return this->_binding;
        }

        [[nodiscard]] static StrongTypedProperty parse(std::string_view &&) { throw EngineError::notImplemented(); }

    private:
        PropertyType _propertyType;
        T _constant;
        std::string _binding;
    };

    template <ValueType Type>
    class BindableProperty {
    public:
        explicit BindableProperty(std::string &&binding)
            : _binding(binding) {
            //
        }

        [[nodiscard]] ValueType getValueType() const { return Type; }

        [[nodiscard]] PropertyType getPropertyType() const { return PropertyType::Binding; }

        [[nodiscard]] const std::string &getBinding() const { return this->_binding; }

        [[nodiscard]] static BindableProperty parse(std::string_view &&) { throw EngineError::notImplemented(); }

    private:
        std::string _binding;
    };

    using BooleanProperty = StrongTypedProperty<ValueType::Boolean, bool>;
    using IntegerProperty = StrongTypedProperty<ValueType::Integer, int>;
    using FloatProperty = StrongTypedProperty<ValueType::Float, float>;
    using StringProperty = StrongTypedProperty<ValueType::String, std::string>;
    using ActionProperty = BindableProperty<ValueType::Action>;
    using ObjectProperty = BindableProperty<ValueType::Object>;
    using ListProperty = BindableProperty<ValueType::List>;

    template <>
    BooleanProperty BooleanProperty::parse(std::string_view &&);

    template <>
    IntegerProperty IntegerProperty::parse(std::string_view &&);

    template <>
    FloatProperty FloatProperty::parse(std::string_view &&);

    template <>
    StringProperty StringProperty::parse(std::string_view &&);

    template <>
    ActionProperty ActionProperty::parse(std::string_view &&);

    template <>
    ObjectProperty ObjectProperty::parse(std::string_view &&);

    template <>
    ListProperty ListProperty::parse(std::string_view &&);
} // namespace Penrose

#endif // PENROSE_UI_PROPERTY_HPP
