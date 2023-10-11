#include "UIDrawVisitor.hpp"

#include <imgui.h>

#include <Penrose/Common/EngineError.hpp>

namespace Penrose {

    int textInputCallback(ImGuiInputTextCallbackData *data) {
        auto textInput = reinterpret_cast<TextInput *>(data->UserData);
        auto thresholdSize = static_cast<int>(textInput->getSize()) - 1;

        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize && data->BufTextLen >= thresholdSize) {
            textInput->grow();

            data->Buf = const_cast<char *>(textInput->getText().c_str());
            data->BufSize = static_cast<int>(textInput->getSize());
        }

        return 0;
    }

    void UIDrawVisitor::visit(const std::shared_ptr<Widget> &widget) { // NOLINT(misc-no-recursion)

        if (!widget->getVisible()) {
            return;
        }

        switch (widget->getType()) {
            case WidgetType::Window:
                this->visitWindow(std::dynamic_pointer_cast<Window>(widget));
                break;

            case WidgetType::Label:
                this->visitLabel(std::dynamic_pointer_cast<Label>(widget));
                break;

            case WidgetType::TextInput:
                this->visitTextInput(std::dynamic_pointer_cast<TextInput>(widget));
                break;

            case WidgetType::Button:
                this->visitButton(std::dynamic_pointer_cast<Button>(widget));
                break;

            case WidgetType::DropDown:
                this->visitDropDown(std::dynamic_pointer_cast<DropDown>(widget));
                break;

            case WidgetType::Container:
                this->visitContainer(std::dynamic_pointer_cast<Container>(widget));
                break;

            default:
                throw EngineError("Widget is not supported");
        }
    }

    void UIDrawVisitor::visitWindow(const std::shared_ptr<Window> &window) { // NOLINT(misc-no-recursion)
        if (ImGui::Begin(window->getTitle().c_str())) {

            for (const auto &child: window->getChilds()) {
                this->visit(child);
            }

            ImGui::End();
        }
    }

    void UIDrawVisitor::visitLabel(const std::shared_ptr<Label> &label) {
        ImGui::Text("%s", label->getText().c_str());
    }

    void UIDrawVisitor::visitTextInput(const std::shared_ptr<TextInput> &textInput) {
        auto buffer = const_cast<char *>(textInput->getText().c_str());

        ImGui::InputText("SomeTextInput", buffer, textInput->getSize(), ImGuiInputTextFlags_CallbackResize,
                         textInputCallback, textInput.get());
    }

    void UIDrawVisitor::visitButton(const std::shared_ptr<Button> &button) {
        if (ImGui::Button(button->getTitle().c_str())) {
            button->invoke();
        }
    }

    void UIDrawVisitor::visitDropDown(const std::shared_ptr<DropDown> &dropDown) {

        const char *preview = "None";

        if (dropDown->getSelected().has_value()) {
            for (const auto &[key, value]: dropDown->getItems()) {
                if (!(key == dropDown->getSelected())) {
                    continue;
                }

                preview = value.c_str();

                break;
            }
        }

        if (ImGui::BeginCombo(dropDown->getTitle().c_str(), preview)) {

            for (const auto &[key, value]: dropDown->getItems()) {

                bool selected = dropDown->getSelected() == key;

                if (ImGui::Selectable(value.c_str(), selected)) {
                    dropDown->selected() = key;
                }

                if (selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }
    }

    void UIDrawVisitor::visitContainer(const std::shared_ptr<Container> &container) {
        for (const auto &child: container->getChilds()) {
            this->visit(child);
        }
    }
}