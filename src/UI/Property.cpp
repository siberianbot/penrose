#include <Penrose/UI/Property.hpp>

#include <algorithm>
#include <cctype>
#include <charconv>
#include <optional>

namespace Penrose {

    constexpr static std::string_view BINDING_START = "${";
    constexpr static std::string_view BINDING_END = "}";

    constexpr static std::string_view BOOLEAN_TRUE = "true";
    constexpr static std::string_view BOOLEAN_FALSE = "false";

    std::optional<std::string> tryGetBinding(const std::string_view &value) {
        if (!value.starts_with(BINDING_START) || !value.ends_with(BINDING_END)) {
            return std::nullopt;
        }

        const auto length = value.size() - (BINDING_START.size() + BINDING_END.size());

        if (length == 0) {
            throw EngineError("Property has empty binding path");
        }

        return std::string(value.substr(BINDING_START.size(), length));
    }

    bool compare(const std::string_view &lhs, const std::string_view &rhs, bool insensitive = true) {
        if (lhs.size() != rhs.size()) {
            return false;
        }

        return std::ranges::equal(lhs, rhs, [insensitive](const char &l, const char &r) {
            if (insensitive) {
                return std::toupper(l) == std::toupper(r);
            }

            return l == r;
        });
    }

    template <>
    BooleanProperty BooleanProperty::parse(std::string_view &&value) {
        auto binding = tryGetBinding(value);

        if (binding.has_value()) {
            return {PropertyType::Binding, false, std::move(*binding)};
        }

        if (compare(value, BOOLEAN_TRUE)) {
            return {PropertyType::Constant, true, ""};
        }

        if (compare(value, BOOLEAN_FALSE)) {
            return {PropertyType::Constant, false, ""};
        }

        throw EngineError("Invalid boolean \"{}\"", value);
    }

    template <>
    IntegerProperty IntegerProperty::parse(std::string_view &&value) {
        auto binding = tryGetBinding(value);

        if (binding.has_value()) {
            return {PropertyType::Binding, 0, std::move(*binding)};
        }

        int integer;
        auto [_, ec] = std::from_chars(value.data(), value.data() + value.size(), integer);

        if (ec != std::errc()) {
            throw EngineError("Invalid integer \"{}\"", value);
        }

        return {PropertyType::Constant, integer, ""};
    }

    template <>
    FloatProperty FloatProperty::parse(std::string_view &&value) {
        auto binding = tryGetBinding(value);

        if (binding.has_value()) {
            return {PropertyType::Binding, 0, std::move(*binding)};
        }

        float f;
        auto [_, ec] = std::from_chars(value.data(), value.data() + value.size(), f);

        if (ec != std::errc()) {
            throw EngineError("Invalid float \"{}\"", value);
        }

        return {PropertyType::Constant, f, ""};
    }

    template <>
    StringProperty StringProperty::parse(std::string_view &&value) {
        auto binding = tryGetBinding(value);

        if (binding.has_value()) {
            return {PropertyType::Binding, "", std::move(*binding)};
        }

        return {PropertyType::Constant, std::string(value), ""};
    }

    template <>
    ActionProperty ActionProperty::parse(std::string_view &&value) {
        auto binding = tryGetBinding(value);

        if (!binding.has_value()) {
            throw EngineError("Action requires binding");
        }

        return ActionProperty(std::move(*binding));
    }

    template <>
    ObjectProperty ObjectProperty::parse(std::string_view &&value) {
        auto binding = tryGetBinding(value);

        if (!binding.has_value()) {
            throw EngineError("Object requires binding");
        }

        return ObjectProperty(std::move(*binding));
    }

    template <>
    ListProperty ListProperty::parse(std::string_view &&value) {
        auto binding = tryGetBinding(value);

        if (!binding.has_value()) {
            throw EngineError("List requires binding");
        }

        return ListProperty(std::move(*binding));
    }
} // namespace Penrose
