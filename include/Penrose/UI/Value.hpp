#ifndef PENROSE_UI_VALUE_HPP
#define PENROSE_UI_VALUE_HPP

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <Penrose/UI/ValueType.hpp>

namespace Penrose {

    class PENROSE_API Value {
    public:
        Value() = default;
        Value(const Value &) = default;
        Value(Value &&) = default;
        Value &operator=(const Value &) = default;
        Value &operator=(Value &&) = default;

        virtual ~Value() = default;

        [[nodiscard]] virtual ValueType getType() const = 0;
    };

    template <ValueType Type, typename T>
    class PENROSE_API StrongTypedValue final: public Value {
    public:
        using Getter = std::function<T()>;
        using Setter = std::function<void(T)>;

        explicit StrongTypedValue(T value, bool constant = false)
            : _value({constant, value}),
              _getter([this]() { return this->_value->value; }),
              _setter([this](T value) {
                  if (this->_value->constant) {
                      return;
                  }

                  this->_value->value = value;
              }) {
            //
        }

        StrongTypedValue(Getter &&getter, Setter &&setter)
            : _value(std::nullopt),
              _getter(getter),
              _setter(setter) {
            //
        }

        ~StrongTypedValue() override = default;

        [[nodiscard]] ValueType getType() const override { return Type; }

        [[nodiscard]] T getValue() { return this->_getter(); }

        void setValue(T value) { return this->_setter(value); }

    private:
        struct InnerValue {
            bool constant;
            T value;
        };

        std::optional<InnerValue> _value;
        Getter _getter;
        Setter _setter;
    };

    using BooleanValue = StrongTypedValue<ValueType::Boolean, bool>;
    using IntegerValue = StrongTypedValue<ValueType::Integer, int>;
    using FloatValue = StrongTypedValue<ValueType::Float, float>;
    using StringValue = StrongTypedValue<ValueType::String, std::string>;

    class PENROSE_API ActionValue final: public Value {
    public:
        using Delegate = std::function<void()>;

        explicit ActionValue(Delegate &&delegate);

        ~ActionValue() override = default;

        [[nodiscard]] ValueType getType() const override { return ValueType::Action; }

        void invoke() const;

    private:
        Delegate _delegate;
    };

    class PENROSE_API ObjectValue final: public Value {
    public:
        using Container = std::map<std::string, std::shared_ptr<Value>>;

        explicit ObjectValue(Container &&properties = {});

        ~ObjectValue() override = default;

        [[nodiscard]] ValueType getType() const override { return ValueType::Object; }

        template <typename T, typename... Args>
        requires std::is_base_of_v<Value, T> && std::is_constructible_v<T, Args...>
        [[nodiscard]] ObjectValue &property(std::string_view &&name, Args &&...args) {
            return this->property(
                std::forward<decltype(name)>(name),
                std::make_shared<T>(std::forward<decltype(args)>(args)...)
            );
        }

        [[nodiscard]] ObjectValue &property(std::string_view &&name, const std::shared_ptr<Value> &value);

        [[nodiscard]] ObjectValue &property(std::string_view &&name, std::shared_ptr<Value> &&value);

        [[nodiscard]] std::optional<std::shared_ptr<Value>> tryGetProperty(const std::string_view &name) const;

        [[nodiscard]] std::shared_ptr<Value> getPropertyByPath(const std::string_view &path) const;

        [[nodiscard]] const Container &getProperties() const { return this->_properties; }

    private:
        Container _properties;
    };

    class PENROSE_API ListValue final: public Value {
    public:
        using Container = std::vector<std::shared_ptr<ObjectValue>>;

        explicit ListValue(Container &&items = {});

        ~ListValue() override = default;

        [[nodiscard]] ValueType getType() const override { return ValueType::List; }

        ListValue &push(std::shared_ptr<ObjectValue> &&item);

        [[nodiscard]] const Container &getItems() const { return this->_items; }

        [[nodiscard]] Container &getItems() { return this->_items; }

    private:
        Container _items;
    };
}

#endif // PENROSE_UI_VALUE_HPP
