#ifndef PENROSE_UI_VALUE_BINDING_HPP
#define PENROSE_UI_VALUE_BINDING_HPP

#include <map>
#include <tuple>

#include <Penrose/UI/Value.hpp>
#include <Penrose/UI/ValueProxy.hpp>

namespace Penrose {

    using ValueBindingKey = std::tuple<const ObjectValueProxy *, const Widget *, std::size_t>;

    struct ValueBinding {
        ValueSource source;
        std::optional<ValueProxy *> proxy;
    };

    using ValueBindings = std::map<ValueBindingKey, ValueBinding>;

    template<typename T, ValueType Type>
    void addStrongTypedBinding(ValueBindings &bindings, const Widget *widget, const ObjectValueProxy *context,
                               const StrongTypedValue<T, Type> *value) {
        auto address = reinterpret_cast<std::size_t>(value);

        auto key = std::make_tuple(context, widget, address);
        auto binding = ValueBinding{
                .source = value->getSource(),
                .proxy = value->getSource() == ValueSource::Binding
                         ? std::make_optional(context->getPropertyByPath(value->getBinding()))
                         : std::nullopt
        };

        bindings.emplace(key, binding);
    }

    template<ValueType Type>
    ValueProxy *addBindOnlyBinding(ValueBindings &bindings, const Widget *widget, const ObjectValueProxy *context,
                                   const BindOnlyValue<Type> *value) {
        auto address = reinterpret_cast<std::size_t>(value);
        auto proxy = context->getPropertyByPath(value->getBinding());

        auto key = std::make_tuple(context, widget, address);
        auto binding = ValueBinding{
                .source = ValueSource::Binding,
                .proxy = proxy
        };

        bindings.emplace(key, binding);

        return proxy;
    }
}

#endif // PENROSE_UI_VALUE_BINDING_HPP
