#include <Penrose/UI/WidgetValue.hpp>

#include <algorithm>
#include <cctype>
#include <charconv>
#include <optional>

namespace Penrose {

    constexpr static const std::string_view BINDING_START = "${";
    constexpr static const std::string_view BINDING_END = "}";

    constexpr static const std::string_view BOOLEAN_TRUE = "true";
    constexpr static const std::string_view BOOLEAN_FALSE = "false";

    std::optional<std::string> tryGetBinding(const std::string_view &value) {
        if (!value.starts_with(BINDING_START) || !value.starts_with(BINDING_END)) {
            return std::nullopt;
        }

        auto length = value.size() - (BINDING_START.size() + BINDING_END.size());

        if (length == 0) {
            throw EngineError("Value has empty binding");
        }

        return std::string(value.substr(BINDING_START.size(), length));
    }

    bool compare(const std::string_view &lhs, const std::string_view &rhs, bool insensitive = true) {
        if (lhs.size() != rhs.size()) {
            return false;
        }

        return std::equal(lhs.begin(), lhs.end(),
                          rhs.begin(), rhs.end(),
                          [insensitive](const char &l, const char &r) {
                              if (insensitive) {
                                  return std::toupper(l) == std::toupper(r);
                              } else {
                                  return l == r;
                              }
                          });
    }

    template<>
    BooleanWidgetValue BooleanWidgetValue::parse(std::string_view &&value) {
        auto binding = tryGetBinding(value);

        if (binding.has_value()) {
            return {WidgetValueSource::Binding, false, std::move(*binding)};
        }

        if (compare(value, BOOLEAN_TRUE)) {
            return {WidgetValueSource::Constant, true, ""};
        }

        if (compare(value, BOOLEAN_FALSE)) {
            return {WidgetValueSource::Constant, false, ""};
        }

        throw EngineError("Invalid boolean value \"{}\"", value);
    }

    template<>
    IntegerWidgetValue IntegerWidgetValue::parse(std::string_view &&value) {
        auto binding = tryGetBinding(value);

        if (binding.has_value()) {
            return {WidgetValueSource::Binding, 0, std::move(*binding)};
        }

        int integer;
        auto [_, ec] = std::from_chars(value.data(), value.data() + value.size(), integer);

        if (ec != std::errc()) {
            throw EngineError("Invalid integer value \"{}\"", value);
        }

        return {WidgetValueSource::Constant, integer, ""};
    }

    template<>
    FloatWidgetValue FloatWidgetValue::parse(std::string_view &&value) {
        auto binding = tryGetBinding(value);

        if (binding.has_value()) {
            return {WidgetValueSource::Binding, 0, std::move(*binding)};
        }

        float f;
        auto [_, ec] = std::from_chars(value.data(), value.data() + value.size(), f);

        if (ec != std::errc()) {
            throw EngineError("Invalid float value \"{}\"", value);
        }

        return {WidgetValueSource::Constant, f, ""};
    }

    template<>
    StringWidgetValue StringWidgetValue::parse(std::string_view &&value) {
        auto binding = tryGetBinding(value);

        if (binding.has_value()) {
            return {WidgetValueSource::Binding, "", std::move(*binding)};
        }

        return {WidgetValueSource::Constant, std::string(value), ""};
    }

    template<>
    ActionWidgetValue ActionWidgetValue::parse(std::string_view &&value) {
        auto binding = tryGetBinding(value);

        if (!binding.has_value()) {
            throw EngineError("Action requires binding");
        }

        return ActionWidgetValue(std::move(*binding));
    }

    template<>
    ObjectWidgetValue ObjectWidgetValue::parse(std::string_view &&value) {
        auto binding = tryGetBinding(value);

        if (!binding.has_value()) {
            throw EngineError("Object requires binding");
        }

        return ObjectWidgetValue(std::move(*binding));
    }

    template<>
    ListWidgetValue ListWidgetValue::parse(std::string_view &&value) {
        auto binding = tryGetBinding(value);

        if (!binding.has_value()) {
            throw EngineError("List requires binding");
        }

        return ListWidgetValue(std::move(*binding));
    }
}
