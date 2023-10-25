#ifndef PENROSE_BUILTIN_PENROSE_ECS_VIEW_COMPONENT_HPP
#define PENROSE_BUILTIN_PENROSE_ECS_VIEW_COMPONENT_HPP

#include <string>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>

namespace Penrose {

    class ViewComponent : public Component<ViewComponent> {
    public:
        ~ViewComponent() override = default;

        [[nodiscard]] std::string &getRenderList() { return this->_renderList; }

    private:
        std::string _renderList = "Default";
    };

    using ViewComponentFactory = GenericComponentFactory<ViewComponent>;
}

#endif // PENROSE_BUILTIN_PENROSE_ECS_VIEW_COMPONENT_HPP
