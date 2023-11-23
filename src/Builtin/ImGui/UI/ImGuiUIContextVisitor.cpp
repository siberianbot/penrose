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
#include <Penrose/UI/Widgets/MenuBar.hpp>
#include <Penrose/UI/Widgets/MenuEntry.hpp>
#include <Penrose/UI/Widgets/MenuSection.hpp>
#include <Penrose/UI/Widgets/Select.hpp>
#include <Penrose/UI/Widgets/Window.hpp>

namespace Penrose {

    ImGuiUIContextVisitor::ImGuiUIContextVisitor() {
        this->_widgetVisitors[WidgetType::Window] = [this](WidgetVisitContext &context) {
            const auto window = dynamic_cast<const Window *>(context.widget);

            const auto title =
                context.uiContext->resolveProperty<StringValue>(context.valueContext, &window->getTitle())->getValue();

            const ObjectValue *nextContext =
                window->getContext().has_value()
                    ? context.uiContext->resolveProperty<ObjectValue>(context.valueContext, &(*window->getContext()))
                    : context.valueContext;

            const auto opened = window->getOpened().has_value()
                                    ? std::optional(context.uiContext->resolveProperty<BooleanValue>(
                                          context.valueContext,
                                          &(*window->getOpened())
                                      ))
                                    : std::nullopt;

            bool openedValue = opened.has_value() ? (*opened)->getValue() : true;
            const bool originalValue = openedValue;

            if (!openedValue) {
                return;
            }

            if (ImGui::Begin(
                    title.c_str(),
                    opened.has_value() ? &openedValue : nullptr,
                    ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar
                )) {
                for (const auto &child: window->getChildren()) {
                    auto childContext = WidgetVisitContext {
                        .uiContext = context.uiContext,
                        .valueContext = nextContext,
                        .widget = child.get(),
                        .isTopLevel = false
                    };

                    this->visit(childContext);
                }
            }

            ImGui::End();

            if (opened.has_value() && openedValue != originalValue) {
                (*opened)->setValue(openedValue);
            }
        };

        this->_widgetVisitors[WidgetType::Button] = [](WidgetVisitContext &context) {
            const auto button = dynamic_cast<const Button *>(context.widget);

            const auto title =
                context.uiContext->resolveProperty<StringValue>(context.valueContext, &button->getTitle())->getValue();

            std::optional<ActionValue *> action =
                button->getAction().has_value()
                    ? std::optional(
                          context.uiContext->resolveProperty<ActionValue>(context.valueContext, &*button->getAction())
                      )
                    : std::nullopt;

            if (ImGui::Button(title.c_str()) && action.has_value()) {
                (*action)->invoke();
            }
        };

        this->_widgetVisitors[WidgetType::Checkbox] = [](WidgetVisitContext &context) {
            const auto checkbox = dynamic_cast<const Checkbox *>(context.widget);

            const auto text =
                context.uiContext->resolveProperty<StringValue>(context.valueContext, &checkbox->getText())->getValue();
            auto checked =
                context.uiContext->resolveProperty<BooleanValue>(context.valueContext, &checkbox->getChecked());

            bool state = checked->getValue();

            if (ImGui::Checkbox(text.c_str(), &state)) {
                checked->setValue(state);
            }
        };

        this->_widgetVisitors[WidgetType::Input] = [this](WidgetVisitContext &context) {
            const auto input = dynamic_cast<const Input *>(context.widget);

            auto text = context.uiContext->resolveProperty<StringValue>(context.valueContext, &input->getText());
            auto textStr = text->getValue();

            const auto tag = this->getTag(context.valueContext, context.widget);

            if (ImGui::InputText(tag.c_str(), &textStr)) {
                text->setValue(textStr);
            }
        };

        this->_widgetVisitors[WidgetType::Label] = [this](WidgetVisitContext &context) {
            const auto label = dynamic_cast<const Label *>(context.widget);

            const auto text =
                context.uiContext->resolveProperty<StringValue>(context.valueContext, &label->getText())->getValue();

            const auto tag = this->getTag(context.valueContext, context.widget);

            ImGui::LabelText(tag.c_str(), "%s", text.c_str());
        };

        this->_widgetVisitors[WidgetType::Group] = [this](WidgetVisitContext &context) {
            const auto group = dynamic_cast<const Group *>(context.widget);

            const ObjectValue *nextContext =
                group->getContext().has_value()
                    ? context.uiContext->resolveProperty<ObjectValue>(context.valueContext, &(*group->getContext()))
                    : context.valueContext;

            for (const auto &child: group->getChildren()) {
                auto childContext = WidgetVisitContext {
                    .uiContext = context.uiContext,
                    .valueContext = nextContext,
                    .widget = child.get(),
                    .isTopLevel = false
                };

                this->visit(childContext);
            }
        };

        this->_widgetVisitors[WidgetType::List] = [this](WidgetVisitContext &context) {
            const auto list = dynamic_cast<const List *>(context.widget);

            const auto items =
                context.uiContext->resolveProperty<ListValue<ObjectValue>>(context.valueContext, &list->getItems());
            const auto selection =
                context.uiContext->resolveProperty<IntegerValue>(context.valueContext, &list->getSelection());

            const auto tag = this->getTag(context.valueContext, context.widget);
            const auto selectedIdx = selection->getValue();

            if (ImGui::BeginListBox(tag.c_str())) {

                const auto itemTemplate = list->getItemTemplate().get();

                for (int idx = 0; idx < items->getItems().size(); ++idx) {
                    const auto &item = items->getItems().at(idx);

                    auto childContext = WidgetVisitContext {
                        .uiContext = context.uiContext,
                        .valueContext = item.get(),
                        .widget = itemTemplate,
                        .isTopLevel = false
                    };

                    this->visit(childContext);

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

        this->_widgetVisitors[WidgetType::Select] = [this](WidgetVisitContext &context) {
            const auto select = dynamic_cast<const Select *>(context.widget);

            const auto items =
                context.uiContext->resolveProperty<ListValue<StringValue>>(context.valueContext, &select->getItems());
            const auto selection =
                context.uiContext->resolveProperty<IntegerValue>(context.valueContext, &select->getSelection());

            const auto selectionIdx = selection->getValue();

            const auto tag = this->getTag(context.valueContext, context.widget);
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

        this->_widgetVisitors[WidgetType::Separator] = [](WidgetVisitContext &) { ImGui::Separator(); };

        this->_widgetVisitors[WidgetType::MenuBar] = [this](WidgetVisitContext &context) {
            const auto menuBar = dynamic_cast<const MenuBar *>(context.widget);

            if (context.isTopLevel ? ImGui::BeginMainMenuBar() : ImGui::BeginMenuBar()) {

                for (const auto &child: menuBar->getChildren()) {
                    auto childContext = WidgetVisitContext {
                        .uiContext = context.uiContext,
                        .valueContext = context.valueContext,
                        .widget = child.get(),
                        .isTopLevel = false
                    };

                    this->visit(childContext);
                }

                context.isTopLevel ? ImGui::EndMainMenuBar() : ImGui::EndMenuBar();
            }
        };

        this->_widgetVisitors[WidgetType::MenuEntry] = [this](WidgetVisitContext &context) {
            const auto menuEntry = dynamic_cast<const MenuEntry *>(context.widget);

            const auto title =
                context.uiContext->resolveProperty<StringValue>(context.valueContext, &menuEntry->getTitle())
                    ->getValue();

            const auto checked =
                context.uiContext->resolveProperty<BooleanValue>(context.valueContext, &menuEntry->getChecked());
            auto checkedValue = checked->getValue();

            const auto action = menuEntry->getAction().has_value()
                                    ? std::optional(context.uiContext->resolveProperty<ActionValue>(
                                          context.valueContext,
                                          &(*menuEntry->getAction())
                                      ))
                                    : std::nullopt;

            if (ImGui::MenuItem(title.c_str(), nullptr, &checkedValue)) {
                checked->setValue(checkedValue);

                if (action.has_value()) {
                    (*action)->invoke();
                }
            }
        };

        this->_widgetVisitors[WidgetType::MenuSection] = [this](WidgetVisitContext &context) {
            const auto menuSection = dynamic_cast<const MenuSection *>(context.widget);

            const auto title =
                context.uiContext->resolveProperty<StringValue>(context.valueContext, &menuSection->getTitle())
                    ->getValue();

            if (ImGui::BeginMenu(title.c_str())) {

                for (const auto &child: menuSection->getChildren()) {
                    auto childContext = WidgetVisitContext {
                        .uiContext = context.uiContext,
                        .valueContext = context.valueContext,
                        .widget = child.get(),
                        .isTopLevel = false
                    };

                    this->visit(childContext);
                }

                ImGui::EndMenu();
            }
        };
    }

    void ImGuiUIContextVisitor::visit(UIContext *uiContext) {
        auto context = WidgetVisitContext {
            .uiContext = uiContext,
            .valueContext = uiContext->getRootContext(),
            .widget = uiContext->getLayout()->getRoot(),
            .isTopLevel = true
        };

        this->visit(context);
    }

    void ImGuiUIContextVisitor::visit(WidgetVisitContext &context) {

        const auto visible =
            context.uiContext->resolveProperty<BooleanValue>(context.valueContext, &context.widget->getVisible());
        const auto enabled =
            context.uiContext->resolveProperty<BooleanValue>(context.valueContext, &context.widget->getEnabled());

        if (!visible->getValue()) {
            return;
        }

        const bool disabled = !enabled->getValue();

        if (disabled) {
            ImGui::BeginDisabled();
        }

        const auto it = this->_widgetVisitors.find(context.widget->getType());

        if (it == this->_widgetVisitors.end()) {
            throw EngineError("Unable to render widget");
        }

        it->second(context);

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
