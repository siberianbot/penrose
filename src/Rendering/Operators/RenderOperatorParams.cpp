#include "RenderOperatorParams.hpp"

#include <fmt/core.h>

#include <Penrose/Common/EngineError.hpp>

#include "src/Utils/OptionalUtils.hpp"

namespace Penrose {

    void RenderOperatorParams::set(const RenderOperatorParams::KeyView &key, const RenderOperatorParams::Value &value) {
        this->_data.insert_or_assign(RenderOperatorParams::Key(key), value);
    }

    std::optional<std::string> RenderOperatorParams::tryGetString(const RenderOperatorParams::KeyView &key) const {
        auto it = this->_data.find(std::string(key));

        if (it == this->_data.end()) {
            return std::nullopt;
        }

        auto value = std::get_if<std::string>(&it->second);
        if (value == nullptr) {
            return std::nullopt;
        }

        return *value;
    }

    std::string RenderOperatorParams::getString(const RenderOperatorParams::KeyView &key) const {
        return orElseThrow(this->tryGetString(key), EngineError(fmt::format("No such parameter {}", key)));
    }

    RenderOperatorParams::ConstIterator RenderOperatorParams::begin() const {
        return this->_data.begin();
    }

    RenderOperatorParams::ConstIterator RenderOperatorParams::end() const {
        return this->_data.end();
    }

    RenderOperatorParams RenderOperatorParams::merge(const RenderOperatorParams &lhs, const RenderOperatorParams &rhs) {
        RenderOperatorParams result = lhs;

        for (const auto &[key, value]: rhs) {
            result.set(key, value);
        }

        return result;
    }
}
