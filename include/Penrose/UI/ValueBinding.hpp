#ifndef PENROSE_UI_VALUE_BINDING_HPP
#define PENROSE_UI_VALUE_BINDING_HPP

#include <map>
#include <memory>
#include <optional>
#include <tuple>

#include <Penrose/UI/Value.hpp>
#include <Penrose/UI/ValueProxy.hpp>

namespace Penrose {

    using ValueBindingKey = std::tuple<const ObjectValueProxy *, const Widget *, std::size_t>;

    template<typename T, ValueType Type>
    ValueBindingKey makeValueBindingKey(const ObjectValueProxy *context, const Widget *widget,
                                        const StrongTypedValue<T, Type> *value) {
        return std::make_tuple(context, widget, reinterpret_cast<std::size_t>(value));
    }

    template<ValueType Type>
    ValueBindingKey makeValueBindingKey(const ObjectValueProxy *context, const Widget *widget,
                                        const BindOnlyValue<Type> *value) {
        return std::make_tuple(context, widget, reinterpret_cast<std::size_t>(value));
    }

    using ValueBindings = std::map<ValueBindingKey, std::shared_ptr<ValueProxy>>;

    template<typename T, ValueType Type>
    void addStrongTypedBinding(ValueBindings &bindings, const Widget *widget, const ObjectValueProxy *context,
                               const StrongTypedValue<T, Type> *value) {
        auto key = makeValueBindingKey(context, widget, value);
        auto proxy = value->getSource() == ValueSource::Binding
                     ? context->getPropertyByPath(value->getBinding())
                     : std::make_shared<StrongTypedValueProxy<T, Type>>(value->getConstant(), true);

        bindings.emplace(key, proxy);
    }

    template<ValueType Type>
    std::optional<std::shared_ptr<ValueProxy>> addBindOnlyBinding(ValueBindings &bindings, const Widget *widget,
                                                                  const ObjectValueProxy *context,
                                                                  const BindOnlyValue<Type> *value) {
        if (!value->getBinding().has_value()) {
            return std::nullopt;
        }

        auto key = makeValueBindingKey(context, widget, value);
        auto proxy = context->getPropertyByPath(*value->getBinding());

        bindings.emplace(key, proxy);

        return proxy;
    }
}

#endif // PENROSE_UI_VALUE_BINDING_HPP
