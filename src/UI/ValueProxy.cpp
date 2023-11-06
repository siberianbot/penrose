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

    ObjectValueProxy &ObjectValueProxy::property(std::string_view &&name, std::shared_ptr<ValueProxy> &&value) {
        this->_properties.emplace(name, std::forward<decltype(value)>(value));

        return *this;
    }

    std::optional<std::shared_ptr<ValueProxy>> ObjectValueProxy::tryGetProperty(const std::string_view &name) const {
        auto it = this->_properties.find(std::string(name));

        if (it == this->_properties.end()) {
            return std::nullopt;
        }

        return it->second;
    }

    std::shared_ptr<ValueProxy> ObjectValueProxy::getPropertyByPath(const std::string_view &path) const {
        std::vector<std::string_view> properties;

        std::string_view remain = path;
        std::string_view::size_type idx = remain.find('.');

        do {
            properties.emplace_back(remain.substr(0, idx));
            remain = remain.substr(idx + 1);
        } while ((idx = remain.find('.')) && idx != std::string_view::npos);

        auto currentObject = this;
        std::optional<std::shared_ptr<ValueProxy>> valueProxy;

        for (std::uint32_t propertyIdx = 0; propertyIdx < properties.size(); ++propertyIdx) {
            auto property = properties.at(propertyIdx);
            valueProxy = currentObject->tryGetProperty(property);

            if (!valueProxy.has_value()) {
                throw EngineError("No such property {}", property);
            }

            if (propertyIdx == properties.size() - 1) {
                return *valueProxy;
            }

            currentObject = dynamic_cast<ObjectValueProxy *>(valueProxy->get());

            if (currentObject == nullptr) {
                throw EngineError("Property {} is not an object", property);
            }
        }
    }
}
