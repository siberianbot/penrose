#ifndef PENROSE_BUILTIN_PENROSE_ECS_VIEW_COMPONENT_HPP
#define PENROSE_BUILTIN_PENROSE_ECS_VIEW_COMPONENT_HPP

#include <string>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ViewComponent : public Component {
    public:
        ~ViewComponent() override = default;

        [[nodiscard]] std::string &getRenderList() { return this->_renderList; }

        [[nodiscard]] std::string getName() const override { return name(); }

        [[nodiscard]] constexpr static std::string name() { return "View"; }

    private:
        std::string _renderList = "Default";
    };

    class ViewComponentFactory : public Resource, public GenericComponentFactory<ViewComponent> {
    public:
        ~ViewComponentFactory() override = default;
    };
}

#endif // PENROSE_BUILTIN_PENROSE_ECS_VIEW_COMPONENT_HPP
