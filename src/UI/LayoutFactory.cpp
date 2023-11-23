#include <Penrose/UI/LayoutFactory.hpp>

#include <optional>

#include <libxml++/libxml++.h>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/UI/Widgets/Button.hpp>
#include <Penrose/UI/Widgets/Checkbox.hpp>
#include <Penrose/UI/Widgets/Group.hpp>
#include <Penrose/UI/Widgets/Input.hpp>
#include <Penrose/UI/Widgets/Label.hpp>
#include <Penrose/UI/Widgets/List.hpp>
#include <Penrose/UI/Widgets/Select.hpp>
#include <Penrose/UI/Widgets/Window.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

namespace Penrose {

    template <typename T>
    concept IsValueParsable = requires(T, std::string_view &&value) {
        { T::parse(std::forward<decltype(value)>(value)) } -> std::same_as<T>;
    };

    template <IsValueParsable T>
    std::optional<T> tryGetAttribute(const xmlpp::Element *element, std::string &&name) {
        auto attribute = element->get_attribute(name);

        if (attribute == nullptr) {
            return std::nullopt;
        }

        return T::parse(attribute->get_value());
    }

    template <IsValueParsable T>
    std::optional<T> getOptionalAttribute(const xmlpp::Element *element, std::string &&name) {
        return tryGetAttribute<T>(element, std::forward<decltype(name)>(name));
    }

    template <IsValueParsable T>
    T getOptionalAttribute(const xmlpp::Element *element, std::string &&name, T &&defaultValue) {
        return getOptionalAttribute<T>(element, std::forward<decltype(name)>(name)).value_or(defaultValue);
    }

    template <IsValueParsable T>
    T getRequiredAttribute(const xmlpp::Element *element, std::string &&name) {
        auto value = tryGetAttribute<T>(element, std::string(name));

        if (!value.has_value()) {
            throw EngineError("Element {} requires attribute {}", element->get_name(), name);
        }

        return *value;
    }

    LayoutFactory::LayoutFactory() {
        this->_factories["window"] = [this](const xmlpp::Element *element) -> Widget * {
            auto args = Window::Args {
                .enabled = getOptionalAttribute(element, "enabled", BooleanProperty(true)),
                .visible = getOptionalAttribute(element, "visible", BooleanProperty(true)),
                .title = getRequiredAttribute<StringProperty>(element, "title"),
                .context = getOptionalAttribute<ObjectProperty>(element, "context"),
                .opened = getOptionalAttribute<BooleanProperty>(element, "opened")
            };

            for (const xmlpp::Node *childNode: element->get_children()) {
                auto childElement = dynamic_cast<const xmlpp::Element *>(childNode);

                if (childElement == nullptr) {
                    continue;
                }

                Widget *childWidget;

                try {
                    childWidget = this->makeWidget(childElement);
                } catch (...) {
                    std::throw_with_nested(EngineError("Invalid child node {}", childNode->get_path()));
                }

                args.children.emplace_back(childWidget);
            }

            return new Window(std::move(args));
        };

        this->_factories["label"] = [](const xmlpp::Element *element) -> Widget * {
            auto args = Label::Args {
                .enabled = getOptionalAttribute(element, "enabled", BooleanProperty(true)),
                .visible = getOptionalAttribute(element, "visible", BooleanProperty(true)),
                .text = getRequiredAttribute<StringProperty>(element, "text")
            };

            return new Label(std::move(args));
        };

        this->_factories["button"] = [](const xmlpp::Element *element) -> Widget * {
            auto args = Button::Args {
                .enabled = getOptionalAttribute(element, "enabled", BooleanProperty(true)),
                .visible = getOptionalAttribute(element, "visible", BooleanProperty(true)),
                .title = getRequiredAttribute<StringProperty>(element, "title"),
                .action = getOptionalAttribute<ActionProperty>(element, "action")
            };

            return new Button(std::move(args));
        };

        this->_factories["input"] = [](const xmlpp::Element *element) -> Widget * {
            auto args = Input::Args {
                .enabled = getOptionalAttribute(element, "enabled", BooleanProperty(true)),
                .visible = getOptionalAttribute(element, "visible", BooleanProperty(true)),
                .text = getRequiredAttribute<StringProperty>(element, "text")
            };

            return new Input(std::move(args));
        };

        this->_factories["checkbox"] = [](const xmlpp::Element *element) -> Widget * {
            auto args = Checkbox::Args {
                .enabled = getOptionalAttribute(element, "enabled", BooleanProperty(true)),
                .visible = getOptionalAttribute(element, "visible", BooleanProperty(true)),
                .text = getRequiredAttribute<StringProperty>(element, "text"),
                .checked = getRequiredAttribute<BooleanProperty>(element, "checked")
            };

            return new Checkbox(std::move(args));
        };

        auto groupFactory = [this](const xmlpp::Element *element) -> Widget * {
            auto args = Group::Args {
                .enabled = getOptionalAttribute(element, "enabled", BooleanProperty(true)),
                .visible = getOptionalAttribute(element, "visible", BooleanProperty(true)),
                .context = getOptionalAttribute<ObjectProperty>(element, "context")
            };

            for (const xmlpp::Node *childNode: element->get_children()) {
                auto childElement = dynamic_cast<const xmlpp::Element *>(childNode);

                if (childElement == nullptr) {
                    continue;
                }

                Widget *childWidget;

                try {
                    childWidget = this->makeWidget(childElement);
                } catch (...) {
                    std::throw_with_nested(EngineError("Invalid child node {}", childNode->get_path()));
                }

                args.children.emplace_back(childWidget);
            }

            return new Group(std::move(args));
        };

        this->_factories["group"] = groupFactory;
        this->_factories["list-item"] = groupFactory;

        this->_factories["list"] = [this](const xmlpp::Element *element) -> Widget * {
            auto args = List::Args {
                .enabled = getOptionalAttribute(element, "enabled", BooleanProperty(true)),
                .visible = getOptionalAttribute(element, "visible", BooleanProperty(true)),
                .items = getRequiredAttribute<ListProperty>(element, "items"),
                .selection = getRequiredAttribute<IntegerProperty>(element, "selection")
            };

            auto itemTemplate = dynamic_cast<const xmlpp::Element *>(element->get_first_child("list-item"));

            if (itemTemplate == nullptr) {
                throw EngineError("List widget requires list-item as item template");
            }

            args.itemTemplate = std::unique_ptr<Widget>(this->makeWidget(itemTemplate));

            return new List(std::move(args));
        };

        this->_factories["select"] = [this](const xmlpp::Element *element) -> Widget * {
            auto args = Select::Args {
                .enabled = getOptionalAttribute(element, "enabled", BooleanProperty(true)),
                .visible = getOptionalAttribute(element, "visible", BooleanProperty(true)),
                .items = getRequiredAttribute<ListProperty>(element, "items"),
                .selection = getRequiredAttribute<IntegerProperty>(element, "selection")
            };

            return new Select(std::move(args));
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

        return new Layout(std::unique_ptr<Widget>(root));
    }

    Widget *LayoutFactory::makeWidget(const xmlpp::Element *element) {
        auto name = element->get_name();

        auto factory = orElseThrow(tryGet(this->_factories, name), EngineError("No factory for widget {}", name));

        return factory(element);
    }
}
