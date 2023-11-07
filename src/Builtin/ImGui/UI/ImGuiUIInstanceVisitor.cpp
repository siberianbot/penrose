#include "ImGuiUIInstanceVisitor.hpp"

#include <fmt/core.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/UI/Widgets/Button.hpp>
#include <Penrose/UI/Widgets/Checkbox.hpp>
#include <Penrose/UI/Widgets/Input.hpp>
#include <Penrose/UI/Widgets/Label.hpp>
#include <Penrose/UI/Widgets/Window.hpp>

namespace Penrose {

    template<typename T, ValueType Type>
    std::shared_ptr<StrongTypedValueProxy<T, Type>> resolve(const UIInstance *instance,
                                                            const ObjectValueProxy *context,
                                                            const Widget *widget,
                                                            const StrongTypedValue<T, Type> *value) {
        auto key = makeValueBindingKey(context, widget, value);
        auto proxy = instance->getBinding(key);

        return std::dynamic_pointer_cast<StrongTypedValueProxy<T, Type>>(proxy);
    }

    template<typename T, ValueType Type>
    requires std::is_base_of_v<ValueProxy, T>
    std::optional<std::shared_ptr<T>> resolve(const UIInstance *instance, const ObjectValueProxy *context,
                                              const Widget *widget, const BindOnlyValue<Type> *value) {
        if (!value->getBinding().has_value()) {
            return std::nullopt;
        }

        auto key = makeValueBindingKey(context, widget, value);
        auto proxy = instance->getBinding(key);

        return std::dynamic_pointer_cast<T>(proxy);
    }

    ImGuiUIInstanceVisitor::ImGuiUIInstanceVisitor() {
        this->_widgetVisitors[WidgetType::Window] = [this](const UIInstance *instance, const ObjectValueProxy *context,
                                                           const Widget *widget) {
            auto window = dynamic_cast<const Window *>(widget);

            auto title = resolve(instance, context, window, &window->getTitle())->getValue();
            auto nestedContext = resolve<ObjectValueProxy>(instance, context, window, &window->getContext());

            if (ImGui::Begin(title.c_str() /* TODO: , open/close */)) {
                for (const auto &child: window->getChildren()) {
                    this->visit(instance,
                                nestedContext.has_value() ? nestedContext->get() : context,
                                child.get());
                }

                ImGui::End();
            }
        };

        this->_widgetVisitors[WidgetType::Button] = [](const UIInstance *instance, const ObjectValueProxy *context,
                                                       const Widget *widget) {
            auto button = dynamic_cast<const Button *>(widget);

            auto title = resolve(instance, context, button, &button->getTitle())->getValue();
            auto action = resolve<ActionValueProxy>(instance, context, button, &button->getAction());

            if (ImGui::Button(title.c_str()) && action.has_value()) {
                (*action)->invoke();
            }
        };

        this->_widgetVisitors[WidgetType::Checkbox] = [](const UIInstance *instance, const ObjectValueProxy *context,
                                                         const Widget *widget) {
            auto checkbox = dynamic_cast<const Checkbox *>(widget);

            auto text = resolve(instance, context, checkbox, &checkbox->getText())->getValue();
            auto checked = resolve(instance, context, checkbox, &checkbox->getChecked());

            bool state = checked->getValue();

            if (ImGui::Checkbox(text.c_str(), &state)) {
                checked->setValue(state);
            }
        };

        this->_widgetVisitors[WidgetType::Input] = [this](const UIInstance *instance, const ObjectValueProxy *context,
                                                          const Widget *widget) {
            auto input = dynamic_cast<const Input *>(widget);

            auto text = resolve(instance, context, input, &input->getText());
            auto textStr = text->getValue();

            auto tag = this->getTag(context, widget);

            if (ImGui::InputText(tag.c_str(), &textStr)) {
                text->setValue(textStr);
            }
        };

        this->_widgetVisitors[WidgetType::Label] = [this](const UIInstance *instance, const ObjectValueProxy *context,
                                                          const Widget *widget) {
            auto label = dynamic_cast<const Label *>(widget);

            auto text = resolve(instance, context, label, &label->getText())->getValue();

            auto tag = this->getTag(context, widget);

            ImGui::LabelText(tag.c_str(), "%s", text.c_str());
        };
    }

    void ImGuiUIInstanceVisitor::visit(UIInstance *instance) {
        this->visit(instance, instance->getRootContext(), instance->getLayout()->getRoot());
    }

    void ImGuiUIInstanceVisitor::visit(const UIInstance *instance, const ObjectValueProxy *context,
                                       const Widget *widget) {

        auto visible = resolve(instance, context, widget, &widget->getVisible());
        auto enabled = resolve(instance, context, widget, &widget->getEnabled());

        if (!visible->getValue()) {
            return;
        }

        if (!enabled->getValue()) {
            ImGui::BeginDisabled();
        }

        auto it = this->_widgetVisitors.find(widget->getType());

        if (it == this->_widgetVisitors.end()) {
            throw EngineError("Unable to render widget");
        }

        it->second(instance, context, widget);

        if (!enabled->getValue()) {
            ImGui::EndDisabled();
        }
    }

    const std::string &ImGuiUIInstanceVisitor::getTag(const ObjectValueProxy *context, const Widget *widget) {
        auto tag = Tag(context, widget);

        auto it = this->_tags.find(tag);

        if (it != this->_tags.end()) {
            return it->second;
        }

        auto [newIt, _] = this->_tags.emplace(tag, fmt::format("##{}{}",
                                                               reinterpret_cast<std::size_t>(context),
                                                               reinterpret_cast<std::size_t>(widget)));

        return newIt->second;
    }
}
