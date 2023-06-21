#ifndef PENROSE_RENDERING_OPERATORS_RENDER_OPERATOR_PARAMS_HPP
#define PENROSE_RENDERING_OPERATORS_RENDER_OPERATOR_PARAMS_HPP

#include <optional>
#include <string>
#include <unordered_map>
#include <variant>

namespace Penrose {

    class RenderOperatorParams {
    public:
        using Key = std::string;
        using KeyView = std::string_view;
        using Value = std::variant<std::string>;
        using Container = std::unordered_map<Key, Value>;

    private:
        Container _data;

        void set(const KeyView &key, const Value &value);

    public:
        using ConstIterator = Container::const_iterator;

        void setString(const KeyView &key, const std::string &value) { this->set(key, value); }

        [[nodiscard]] std::optional<std::string> tryGetString(const KeyView &key) const;
        [[nodiscard]] std::string getString(const KeyView &key) const;

        [[nodiscard]] ConstIterator begin() const;
        [[nodiscard]] ConstIterator end() const;

        [[nodiscard]] static RenderOperatorParams empty() { return {}; }

        [[nodiscard]] static RenderOperatorParams merge(const RenderOperatorParams &lhs,
                                                        const RenderOperatorParams &rhs);
    };
}

#endif // PENROSE_RENDERING_OPERATORS_RENDER_OPERATOR_PARAMS_HPP
