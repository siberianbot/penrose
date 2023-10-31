#ifndef PENROSE_UI_VALUE_PROXY_HPP
#define PENROSE_UI_VALUE_PROXY_HPP

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <Penrose/UI/ValueType.hpp>

namespace Penrose {

    class ValueProxy {
    public:
        ValueProxy() = default;
        ValueProxy(const ValueProxy &) = default;
        ValueProxy(ValueProxy &&) = default;
        ValueProxy &operator=(const ValueProxy &) = default;
        ValueProxy &operator=(ValueProxy &&) = default;

        virtual ~ValueProxy() = default;

        [[nodiscard]] virtual ValueType getType() const = 0;
    };

    template<typename T, ValueType Type>
    class StrongTypedValueProxy : public ValueProxy {
    public:
        using Getter = std::function<T &()>;
        using Setter = std::function<void(T)>;

        explicit StrongTypedValueProxy(T value)
                : _getter([value]() { return value; }),
                  _setter([](T) {}) {
            //
        }

        StrongTypedValueProxy(Getter &&getter, Setter &&setter)
                : _getter(getter),
                  _setter(setter) {
            //
        }

        ~StrongTypedValueProxy() override = default;

        [[nodiscard]] ValueType getType() const override { return Type; }

        [[nodiscard]] T &getValue() { return this->_getter(); }

        void setValue(T value) { return this->_setter(value); }

    private:
        Getter _getter;
        Setter _setter;
    };

    using BooleanValueProxy = StrongTypedValueProxy<bool, ValueType::Boolean>;
    using IntegerValueProxy = StrongTypedValueProxy<int, ValueType::Integer>;
    using FloatValueProxy = StrongTypedValueProxy<float, ValueType::Float>;
    using StringValueProxy = StrongTypedValueProxy<std::string, ValueType::String>;

    class ActionValueProxy : public ValueProxy {
    public:
        using Delegate = std::function<void()>;

        explicit ActionValueProxy(Delegate &&delegate);

        ~ActionValueProxy() override = default;

        void invoke();

    private:
        Delegate _delegate;
    };

    class ObjectValueProxy : public ValueProxy {
    public:
        using Container = std::map<std::string, std::unique_ptr<ValueProxy>>;

        explicit ObjectValueProxy(Container &&properties = {});

        template<typename T, typename ...Args>
        requires std::is_base_of_v<ValueProxy, T> && std::is_constructible_v<T, Args...>
        [[nodiscard]] ObjectValueProxy &property(std::string_view &&name, Args &&...args) {
            return this->property(std::forward<decltype(name)>(name),
                                  std::make_shared<T>(std::forward<decltype(args)>(args)...));
        }

        [[nodiscard]] ObjectValueProxy &property(std::string_view &&name, std::unique_ptr<ValueProxy> &&value);

        [[nodiscard]] std::optional<ValueProxy *> tryGetProperty(const std::string_view &name) const;

        [[nodiscard]] ValueProxy *getPropertyByPath(const std::string_view &path) const;

        [[nodiscard]] const Container &getProperties() const { return this->_properties; }

    private:
        Container _properties;
    };

    // TODO: ListValueProxy
}

#endif // PENROSE_UI_VALUE_PROXY_HPP
