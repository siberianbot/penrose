#include <Penrose/UI/ValueProxy.hpp>

#include <cstdint>
#include <vector>

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    ActionValueProxy::ActionValueProxy(ActionValueProxy::Delegate &&delegate)
            : _delegate(delegate) {
        //
    }

    void ActionValueProxy::invoke() {
        this->_delegate();
    }

    ObjectValueProxy::ObjectValueProxy(ObjectValueProxy::Container &&properties)
            : _properties(properties) {
        //
    }

    ObjectValueProxy &ObjectValueProxy::property(std::string_view &&name, std::unique_ptr<ValueProxy> &&value) {
        this->_properties.emplace(name, std::forward<decltype(value)>(value));

        return *this;
    }

    std::optional<ValueProxy *> ObjectValueProxy::tryGetProperty(const std::string_view &name) const {
        auto it = this->_properties.find(std::string(name));

        if (it == this->_properties.end()) {
            return std::nullopt;
        }

        return it->second.get();
    }

    ValueProxy *ObjectValueProxy::getPropertyByPath(const std::string_view &path) const {
        std::vector<std::string_view> properties;

        std::string_view::size_type idx;
        std::string_view remain = path;

        while ((idx = remain.find('.')) && idx != std::string_view::npos) {
            properties.emplace_back(remain.substr(0, idx));
            remain = remain.substr(idx + 1);
        }

        auto currentObject = this;
        for (std::uint32_t propertyIdx = 0; propertyIdx < properties.size(); ++propertyIdx) {
            auto property = properties.at(propertyIdx);
            auto valueProxy = currentObject->tryGetProperty(property);

            if (!valueProxy.has_value()) {
                throw EngineError("No such property {}", property);
            }

            if (propertyIdx == properties.size() - 1) {
                return *valueProxy;
            }

            currentObject = dynamic_cast<ObjectValueProxy *>(*valueProxy);

            if (currentObject == nullptr) {
                throw EngineError("Property {} is not an object", property);
            }
        }
    }
}
