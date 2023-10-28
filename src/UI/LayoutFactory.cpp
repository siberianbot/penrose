#include <Penrose/UI/LayoutFactory.hpp>

#include <optional>

#include <libxml++/libxml++.h>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/UI/Widgets/Window.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

namespace Penrose {

    template<typename T>
    std::optional<Value<T>> tryGetAttribute(const xmlpp::Element *element, std::string &&name) {
        auto attribute = element->get_attribute(name);

        if (attribute == nullptr) {
            return std::nullopt;
        }

        return Value<T>::parse(attribute->get_value());
    }

    template<typename T>
    Value<T> getOptionalAttribute(const xmlpp::Element *element, std::string &&name, T defaultValue) {
        return tryGetAttribute<T>(element, std::forward<decltype(name)>(name))
                .value_or(Value<T>(defaultValue));
    }

    template<typename T>
    Value<T> getRequiredAttribute(const xmlpp::Element *element, std::string &&name) {
        auto value = tryGetAttribute<T>(element, std::string(name));

        if (!value.has_value()) {
            throw EngineError("Element {} requires attribute {}", element->get_name(), name);
        }

        return *value;
    }

    LayoutFactory::LayoutFactory() {
        this->_factories["window"] = [this](const xmlpp::Element *element) {
            return new Window(Window::Args{
                    .visible = getOptionalAttribute(element, "visible", true),
                    .title = getRequiredAttribute<std::string>(element, "title")
            });
        };
    }

    Layout *LayoutFactory::makeLayout(std::vector<unsigned char> &&content) {

        xmlpp::DomParser parser;

        try {
            parser.parse_memory_raw(content.data(), content.size());
        } catch (...) {
            std::throw_with_nested(EngineError("Failed to parse layout"));
        }

        if (parser.get_document() == nullptr || parser.get_document()->get_root_node() == nullptr) {
            throw EngineError("Layout is empty");
        }

        auto root = this->makeWidget(parser.get_document()->get_root_node());

        return new Layout(std::unique_ptr<WidgetBase>(root));
    }

    WidgetBase *LayoutFactory::makeWidget(const xmlpp::Element *element) {
        auto name = element->get_name();

        auto factory = orElseThrow(
                tryGet(this->_factories, name),
                EngineError("No factory for widget {}", name)
        );

        return factory(element);
    }
}
