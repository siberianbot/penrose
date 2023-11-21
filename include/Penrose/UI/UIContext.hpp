#ifndef PENROSE_UI_UI_CONTEXT_HPP
#define PENROSE_UI_UI_CONTEXT_HPP

#include <map>
#include <memory>
#include <tuple>

#include <Penrose/Assets/UILayoutAsset.hpp>
#include <Penrose/UI/Property.hpp>
#include <Penrose/UI/Value.hpp>

namespace Penrose {

    class PENROSE_API UIContext {
    public:
        using PropertyAddress = std::size_t;
        using BindingKey = std::tuple<const ObjectValue *, PropertyAddress>;

        UIContext(std::shared_ptr<UILayoutAsset> &&layoutAsset, std::unique_ptr<ObjectValue> &&rootContext)
            : _layoutAsset(std::forward<decltype(layoutAsset)>(layoutAsset)),
              _rootContext(std::forward<decltype(rootContext)>(rootContext)) {
            //
        }

        [[nodiscard]] const Layout *getLayout() const { return this->_layoutAsset->getLayout(); }

        [[nodiscard]] const ObjectValue *getRootContext() const { return this->_rootContext.get(); }

        template <typename V, typename T, ValueType Type>
        requires std::is_base_of_v<Value, V>
        [[nodiscard]] V *resolveProperty(const ObjectValue *context, const StrongTypedProperty<Type, T> *property) {
            auto bindingKey = BindingKey {context, reinterpret_cast<PropertyAddress>(property)};

            if (const auto it = this->_bindings.find(bindingKey); it != this->_bindings.end()) {
                return dynamic_cast<V *>(it->second);
            }

            Value *value;

            switch (property->getPropertyType()) {
                case PropertyType::Constant:
                    value = new StrongTypedValue<Type, T>(property->getConstant(), true);
                    break;

                case PropertyType::Binding:
                    value = context->getPropertyByPath(property->getBinding()).get();

                    if (value->getType() != property->getValueType()) {
                        throw EngineError("Type mismatch");
                    }

                    break;

                default:
                    throw EngineError("Property has incompatible type");
            }

            this->_bindings.emplace(bindingKey, value);

            return dynamic_cast<V *>(value);
        }

        template <typename V, ValueType Type>
        requires std::is_base_of_v<Value, V>
        [[nodiscard]] V *resolveProperty(const ObjectValue *context, const BindableProperty<Type> *property) {
            auto bindingKey = BindingKey {context, reinterpret_cast<PropertyAddress>(property)};

            if (const auto it = this->_bindings.find(bindingKey); it != this->_bindings.end()) {
                return dynamic_cast<V *>(it->second);
            }

            Value *value;

            switch (property->getPropertyType()) {
                case PropertyType::Binding:
                    value = context->getPropertyByPath(property->getBinding()).get();

                    if (value->getType() != property->getValueType()) {
                        throw EngineError("Type mismatch");
                    }

                    break;

                default:
                    throw EngineError("Property has incompatible type");
            }

            this->_bindings.emplace(bindingKey, value);

            return dynamic_cast<V *>(value);
        }

    private:
        std::shared_ptr<UILayoutAsset> _layoutAsset;
        std::unique_ptr<ObjectValue> _rootContext;

        std::map<BindingKey, Value *> _bindings;
    };
}

#endif // PENROSE_UI_UI_CONTEXT_HPP
