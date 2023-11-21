#ifndef PENROSE_BUILTIN_IMGUI_UI_IMGUI_UI_CONTEXT_VISITOR_HPP
#define PENROSE_BUILTIN_IMGUI_UI_IMGUI_UI_CONTEXT_VISITOR_HPP

#include <functional>
#include <map>
#include <string>
#include <tuple>

#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/UI/UIContext.hpp>
#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class ImGuiUIContextVisitor final: public Resource<ImGuiUIContextVisitor, ResourceGroup::UI> {
    public:
        ImGuiUIContextVisitor();
        ~ImGuiUIContextVisitor() override = default;

        void visit(UIContext *context);

    private:
        using WidgetVisitor = std::function<void(UIContext *, const ObjectValue *, const Widget *)>;
        using Tag = std::tuple<const ObjectValue *, const Widget *>;

        std::map<WidgetType, WidgetVisitor> _widgetVisitors;
        std::map<Tag, std::string> _tags;

        void visit(UIContext *uiContext, const ObjectValue *valueContext, const Widget *widget);

        [[nodiscard]] const std::string &getTag(const ObjectValue *valueContext, const Widget *widget);
    };
}

#endif // PENROSE_BUILTIN_IMGUI_UI_IMGUI_UI_CONTEXT_VISITOR_HPP
