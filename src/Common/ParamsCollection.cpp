#include <Penrose/Common/ParamsCollection.hpp>

#include <fmt/core.h>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

namespace Penrose {

    void ParamsCollection::set(const ParamsCollection::KeyView &key, const ParamsCollection::Value &value) {
        this->_data.insert_or_assign(ParamsCollection::Key(key), value);
    }

    std::optional<std::string> ParamsCollection::tryGetString(const ParamsCollection::KeyView &key) const {
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

    std::string ParamsCollection::getString(const ParamsCollection::KeyView &key) const {
        return orElseThrow(this->tryGetString(key), EngineError(fmt::format("No such parameter {}", key)));
    }

    ParamsCollection::ConstIterator ParamsCollection::begin() const {
        return this->_data.begin();
    }

    ParamsCollection::ConstIterator ParamsCollection::end() const {
        return this->_data.end();
    }

    ParamsCollection ParamsCollection::merge(const ParamsCollection &rhs) const {
        return ParamsCollection::merge(*this, rhs);
    }

    ParamsCollection ParamsCollection::merge(const ParamsCollection &lhs, const ParamsCollection &rhs) {
        ParamsCollection result = lhs;

        for (const auto &[key, value]: rhs) {
            result.set(key, value);
        }

        return result;
    }
}
