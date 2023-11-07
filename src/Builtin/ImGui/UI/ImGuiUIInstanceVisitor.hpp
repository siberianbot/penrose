#ifndef PENROSE_BUILTIN_IMGUI_UI_IMGUI_UI_INSTANCE_VISITOR_HPP
#define PENROSE_BUILTIN_IMGUI_UI_IMGUI_UI_INSTANCE_VISITOR_HPP

#include <functional>
#include <map>
#include <string>
#include <tuple>

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
        using Tag = std::tuple<const ObjectValueProxy *, const Widget *>;

        std::map<WidgetType, WidgetVisitor> _widgetVisitors;
        std::map<Tag, std::string> _tags;

        void visit(const UIInstance *instance, const ObjectValueProxy *context, const Widget *widget);

        [[nodiscard]] const std::string &getTag(const ObjectValueProxy *context, const Widget *widget);
    };
}

#endif // PENROSE_BUILTIN_IMGUI_UI_IMGUI_UI_INSTANCE_VISITOR_HPP
