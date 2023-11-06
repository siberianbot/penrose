#ifndef PENROSE_COMMON_PARAMS_COLLECTION_HPP
#define PENROSE_COMMON_PARAMS_COLLECTION_HPP

#include <optional>
#include <string>
#include <unordered_map>
#include <variant>

#include <Penrose/Api.hpp>

namespace Penrose {

    // TODO: looks shitty
    class PENROSE_API ParamsCollection {
    public:
        using Key = std::string;
        using KeyView = std::string_view;
        using Value = std::variant<std::string>;
        using Container = std::unordered_map<Key, Value>;
        using ConstIterator = Container::const_iterator;

        void setString(const KeyView &key, const std::string &value) { this->set(key, value); }

        void set(const KeyView &key, const Value &value);

        [[nodiscard]] std::optional<std::string> tryGetString(const KeyView &key) const;
        [[nodiscard]] std::string getString(const KeyView &key) const;

        [[nodiscard]] ConstIterator begin() const;
        [[nodiscard]] ConstIterator end() const;

        [[nodiscard]] ParamsCollection merge(const ParamsCollection &rhs) const;

        [[nodiscard]] static ParamsCollection empty() { return {}; }

        [[nodiscard]] static ParamsCollection merge(const ParamsCollection &lhs,
                                                    const ParamsCollection &rhs);

        [[nodiscard]] bool operator==(const ParamsCollection &rhs) const = default;

    private:
        Container _data;
    };
}

#endif // PENROSE_COMMON_PARAMS_COLLECTION_HPP
