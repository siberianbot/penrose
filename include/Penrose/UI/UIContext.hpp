#ifndef PENROSE_UI_UI_CONTEXT_HPP
#define PENROSE_UI_UI_CONTEXT_HPP

#include <list>
#include <map>
#include <memory>
#include <tuple>

#include <Penrose/Assets/UILayoutAsset.hpp>
#include <Penrose/UI/Property.hpp>
#include <Penrose/UI/Value.hpp>

namespace Penrose {

    class PENROSE_API UIContext {
    public:
        using UILayout = std::tuple<std::shared_ptr<Layout>, std::shared_ptr<ObjectValue>>;
        using PropertyAddress = std::size_t;
        using BindingKey = std::tuple<const ObjectValue *, PropertyAddress>;

        void pushLayout(std::shared_ptr<Layout> &&layout, std::shared_ptr<ObjectValue> &&context) {
            this->_layouts.emplace_back(
                std::forward<decltype(layout)>(layout),
                std::forward<decltype(context)>(context)
            );
        }

        [[nodiscard]] const std::list<UILayout> &getLayouts() const { return this->_layouts; }

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
        std::list<UILayout> _layouts;
        std::map<BindingKey, Value *> _bindings;
    };
}

#endif // PENROSE_UI_UI_CONTEXT_HPP
