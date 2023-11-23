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

        void visit(UIContext *uiContext);

    private:
        struct WidgetVisitContext {
            UIContext *uiContext;
            const ObjectValue *valueContext;
            const Widget *widget;
            bool isTopLevel;
        };

        using WidgetVisitor = std::function<void(WidgetVisitContext &)>;
        using Tag = std::tuple<const ObjectValue *, const Widget *>;

        std::map<WidgetType, WidgetVisitor> _widgetVisitors;
        std::map<Tag, std::string> _tags;

        void visit(WidgetVisitContext &context);

        [[nodiscard]] const std::string &getTag(const ObjectValue *valueContext, const Widget *widget);
    };
}

#endif // PENROSE_BUILTIN_IMGUI_UI_IMGUI_UI_CONTEXT_VISITOR_HPP
