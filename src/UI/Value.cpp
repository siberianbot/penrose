#include <Penrose/UI/Value.hpp>

#include <cstdint>
#include <vector>

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    ActionValue::ActionValue(ActionValue::Delegate &&delegate)
        : _delegate(delegate) {
        //
    }

    void ActionValue::invoke() const {
        this->_delegate();
    }

    ObjectValue::ObjectValue(ObjectValue::Container &&properties)
        : _properties(properties) {
        //
    }

    ObjectValue &ObjectValue::property(std::string_view &&name, const std::shared_ptr<Value> &value) {
        this->_properties.emplace(std::forward<decltype(name)>(name), value);

        return *this;
    }

    ObjectValue &ObjectValue::property(std::string_view &&name, std::shared_ptr<Value> &&value) {
        this->_properties.emplace(std::forward<decltype(name)>(name), std::forward<decltype(value)>(value));

        return *this;
    }

    std::optional<std::shared_ptr<Value>> ObjectValue::tryGetProperty(const std::string_view &name) const {
        auto it = this->_properties.find(std::string(name));

        if (it == this->_properties.end()) {
            return std::nullopt;
        }

        return it->second;
    }

    std::shared_ptr<Value> ObjectValue::getPropertyByPath(const std::string_view &path) const {
        std::vector<std::string_view> properties;

        std::string_view remain = path;
        std::string_view::size_type idx = remain.find('.');

        do {
            properties.emplace_back(remain.substr(0, idx));
            remain = remain.substr(idx + 1);
        } while ((idx = remain.find('.')) && idx != std::string_view::npos);

        auto currentObject = this;
        std::optional<std::shared_ptr<Value>> value;

        for (std::uint32_t propertyIdx = 0; propertyIdx < properties.size(); ++propertyIdx) {
            auto property = properties.at(propertyIdx);
            value = currentObject->tryGetProperty(property);

            if (!value.has_value()) {
                throw EngineError("No such property {}", property);
            }

            if (propertyIdx == properties.size() - 1) {
                return *value;
            }

            currentObject = dynamic_cast<ObjectValue *>(value->get());

            if (currentObject == nullptr) {
                throw EngineError("Property {} is not an object", property);
            }
        }

        throw EngineError("Out of reach");
    }

    ListValue::ListValue(ListValue::Container &&items)
        : _items(std::forward<decltype(items)>(items)) {
        //
    }

    ListValue &ListValue::push(std::shared_ptr<ObjectValue> &&item) {
        this->_items.push_back(std::forward<decltype(item)>(item));

        return *this;
    }
}
