#ifndef PENROSE_BUILTIN_IMGUI_UI_IMGUI_UI_INSTANCE_VISITOR_HPP
#define PENROSE_BUILTIN_IMGUI_UI_IMGUI_UI_INSTANCE_VISITOR_HPP

#include <functional>
#include <map>

#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/UI/UIInstance.hpp>
#include <Penrose/UI/ValueProxy.hpp>
#include <Penrose/UI/Widgets/Widget.hpp>

namespace Penrose {

    class ImGuiUIInstanceVisitor : public Resource<ImGuiUIInstanceVisitor, ResourceGroup::UI> {
    public:
        ImGuiUIInstanceVisitor();
        ~ImGuiUIInstanceVisitor() override = default;

        void visit(UIInstance *instance);

    private:
        using WidgetVisitor = std::function<void(const UIInstance *, const ObjectValueProxy *, const Widget *)>;

        std::map<WidgetType, WidgetVisitor> _widgetVisitors;

        void visit(const UIInstance *instance, const ObjectValueProxy *context, const Widget *widget);
    };
}

#endif // PENROSE_BUILTIN_IMGUI_UI_IMGUI_UI_INSTANCE_VISITOR_HPP
