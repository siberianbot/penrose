#include "ImGuiUIContextVisitor.hpp"

#include <fmt/core.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/UI/Widgets/Button.hpp>
#include <Penrose/UI/Widgets/Checkbox.hpp>
#include <Penrose/UI/Widgets/Group.hpp>
#include <Penrose/UI/Widgets/Input.hpp>
#include <Penrose/UI/Widgets/Label.hpp>
#include <Penrose/UI/Widgets/List.hpp>
#include <Penrose/UI/Widgets/Select.hpp>
#include <Penrose/UI/Widgets/Window.hpp>

namespace Penrose {

    ImGuiUIContextVisitor::ImGuiUIContextVisitor() {
        this->_widgetVisitors[WidgetType::Window] = [this](
                                                        UIContext *uiContext,
                                                        const ObjectValue *valueContext,
                                                        const Widget *widget
                                                    ) {
            const auto window = dynamic_cast<const Window *>(widget);

            const auto title = uiContext->resolveProperty<StringValue>(valueContext, &window->getTitle())->getValue();

            const ObjectValue *nextContext =
                window->getContext().has_value()
                    ? uiContext->resolveProperty<ObjectValue>(valueContext, &(*window->getContext()))
                    : valueContext;

            const auto opened =
                window->getOpened().has_value()
                    ? std::optional(uiContext->resolveProperty<BooleanValue>(valueContext, &(*window->getOpened())))
                    : std::nullopt;

            bool openedValue = opened.has_value() ? (*opened)->getValue() : true;
            const bool originalValue = openedValue;

            if (!openedValue) {
                return;
            }

            if (ImGui::Begin(title.c_str(), opened.has_value() ? &openedValue : nullptr, ImGuiWindowFlags_NoCollapse)) {
                for (const auto &child: window->getChildren()) {
                    this->visit(uiContext, nextContext, child.get());
                }
            }

            ImGui::End();

            if (opened.has_value() && openedValue != originalValue) {
                (*opened)->setValue(openedValue);
            }
        };

        this->_widgetVisitors[WidgetType::Button] = [](UIContext *uiContext,
                                                       const ObjectValue *valueContext,
                                                       const Widget *widget) {
            const auto button = dynamic_cast<const Button *>(widget);

            const auto title = uiContext->resolveProperty<StringValue>(valueContext, &button->getTitle())->getValue();

            std::optional<ActionValue *> action =
                button->getAction().has_value()
                    ? std::optional(uiContext->resolveProperty<ActionValue>(valueContext, &*button->getAction()))
                    : std::nullopt;

            if (ImGui::Button(title.c_str()) && action.has_value()) {
                (*action)->invoke();
            }
        };

        this->_widgetVisitors[WidgetType::Checkbox] = [](UIContext *uiContext,
                                                         const ObjectValue *valueContext,
                                                         const Widget *widget) {
            const auto checkbox = dynamic_cast<const Checkbox *>(widget);

            const auto text = uiContext->resolveProperty<StringValue>(valueContext, &checkbox->getText())->getValue();
            auto checked = uiContext->resolveProperty<BooleanValue>(valueContext, &checkbox->getChecked());

            bool state = checked->getValue();

            if (ImGui::Checkbox(text.c_str(), &state)) {
                checked->setValue(state);
            }
        };

        this->_widgetVisitors[WidgetType::Input] =
            [this](UIContext *uiContext, const ObjectValue *valueContext, const Widget *widget) {
                const auto input = dynamic_cast<const Input *>(widget);

                auto text = uiContext->resolveProperty<StringValue>(valueContext, &input->getText());
                auto textStr = text->getValue();

                const auto tag = this->getTag(valueContext, widget);

                if (ImGui::InputText(tag.c_str(), &textStr)) {
                    text->setValue(textStr);
                }
            };

        this->_widgetVisitors[WidgetType::Label] =
            [this](UIContext *uiContext, const ObjectValue *valueContext, const Widget *widget) {
                const auto label = dynamic_cast<const Label *>(widget);

                const auto text = uiContext->resolveProperty<StringValue>(valueContext, &label->getText())->getValue();

                const auto tag = this->getTag(valueContext, widget);

                ImGui::LabelText(tag.c_str(), "%s", text.c_str());
            };

        this->_widgetVisitors[WidgetType::Group] =
            [this](UIContext *uiContext, const ObjectValue *valueContext, const Widget *widget) {
                const auto group = dynamic_cast<const Group *>(widget);

                const ObjectValue *nextContext =
                    group->getContext().has_value()
                        ? uiContext->resolveProperty<ObjectValue>(valueContext, &(*group->getContext()))
                        : valueContext;

                for (const auto &child: group->getChildren()) {
                    this->visit(uiContext, nextContext, child.get());
                }
            };

        this->_widgetVisitors[WidgetType::List] =
            [this](UIContext *uiContext, const ObjectValue *valueContext, const Widget *widget) {
                const auto list = dynamic_cast<const List *>(widget);

                const auto items = uiContext->resolveProperty<ListValue<ObjectValue>>(valueContext, &list->getItems());
                const auto selection = uiContext->resolveProperty<IntegerValue>(valueContext, &list->getSelection());

                const auto tag = this->getTag(valueContext, widget);
                const auto selectedIdx = selection->getValue();

                if (ImGui::BeginListBox(tag.c_str())) {

                    const auto itemTemplate = list->getItemTemplate().get();

                    for (int idx = 0; idx < items->getItems().size(); ++idx) {
                        const auto &item = items->getItems().at(idx);

                        this->visit(uiContext, item.get(), itemTemplate);

                        const auto isSelected = idx == selectedIdx;

                        ImGui::SameLine();

                        if (ImGui::Selectable(
                                this->getTag(item.get(), itemTemplate).c_str(),
                                isSelected,
                                ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap
                            )) {
                            selection->setValue(idx);
                        }

                        if (isSelected) {
                            ImGui::SetItemDefaultFocus();
                        }
                    }

                    ImGui::EndListBox();
                }
            };

        this->_widgetVisitors[WidgetType::Select] =
            [this](UIContext *uiContext, const ObjectValue *valueContext, const Widget *widget) {
                const auto select = dynamic_cast<const Select *>(widget);

                const auto items =
                    uiContext->resolveProperty<ListValue<StringValue>>(valueContext, &select->getItems());
                const auto selection = uiContext->resolveProperty<IntegerValue>(valueContext, &select->getSelection());

                const auto selectionIdx = selection->getValue();

                const auto tag = this->getTag(valueContext, widget);
                const auto previewValue = selectionIdx >= 0 && selectionIdx < items->getItems().size()
                                              ? items->getItems().at(selectionIdx)->getValue()
                                              : "None";

                if (ImGui::BeginCombo(tag.c_str(), previewValue.c_str())) {

                    for (int idx = 0; idx < items->getItems().size(); ++idx) {
                        const auto &item = items->getItems().at(idx);

                        const auto isSelected = selectionIdx == idx;

                        if (ImGui::Selectable(item->getValue().c_str(), isSelected)) {
                            selection->setValue(idx);
                        }

                        if (isSelected) {
                            ImGui::SetItemDefaultFocus();
                        }
                    }

                    ImGui::EndCombo();
                }
            };
    }

    void ImGuiUIContextVisitor::visit(UIContext *context) {
        this->visit(context, context->getRootContext(), context->getLayout()->getRoot());
    }

    void ImGuiUIContextVisitor::visit(UIContext *uiContext, const ObjectValue *valueContext, const Widget *widget) {

        const auto visible = uiContext->resolveProperty<BooleanValue>(valueContext, &widget->getVisible());
        const auto enabled = uiContext->resolveProperty<BooleanValue>(valueContext, &widget->getEnabled());

        if (!visible->getValue()) {
            return;
        }

        const bool disabled = !enabled->getValue();

        if (disabled) {
            ImGui::BeginDisabled();
        }

        const auto it = this->_widgetVisitors.find(widget->getType());

        if (it == this->_widgetVisitors.end()) {
            throw EngineError("Unable to render widget");
        }

        it->second(uiContext, valueContext, widget);

        if (disabled) {
            ImGui::EndDisabled();
        }
    }

    const std::string &ImGuiUIContextVisitor::getTag(const ObjectValue *valueContext, const Widget *widget) {
        auto tag = Tag(valueContext, widget);

        if (const auto it = this->_tags.find(tag); it != this->_tags.end()) {
            return it->second;
        }

        auto [newIt, _] = this->_tags.emplace(
            tag,
            fmt::format("##{}{}", reinterpret_cast<std::size_t>(valueContext), reinterpret_cast<std::size_t>(widget))
        );

        return newIt->second;
    }
}
