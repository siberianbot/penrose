#ifndef PENROSE_BUILTIN_DEBUG_UI_UI_DRAW_VISITOR_HPP
#define PENROSE_BUILTIN_DEBUG_UI_UI_DRAW_VISITOR_HPP

#include <Penrose/Resources/Resource.hpp>

#include <Penrose/Builtin/Debug/UI/Widgets.hpp>

namespace Penrose {

    class UIDrawVisitor : public Resource<UIDrawVisitor, ResourceGroup::Rendering> {
    public:
        ~UIDrawVisitor() override = default;

        void visit(const std::shared_ptr<Widget> &widget);

    private:
        void visitWindow(const std::shared_ptr<Window> &window);
        void visitLabel(const std::shared_ptr<Label> &label);
        void visitTextInput(const std::shared_ptr<TextInput> &textInput);
        void visitButton(const std::shared_ptr<Button> &button);
        void visitDropDown(const std::shared_ptr<DropDown> &dropDown);
        void visitListBox(const std::shared_ptr<ListBox> &listBox);
        void visitContainer(const std::shared_ptr<Container> &container);
    };
}

#endif // PENROSE_BUILTIN_DEBUG_UI_UI_DRAW_VISITOR_HPP
