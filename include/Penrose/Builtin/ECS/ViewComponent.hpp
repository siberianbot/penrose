#ifndef PENROSE_BUILTIN_ECS_VIEW_COMPONENT_HPP
#define PENROSE_BUILTIN_ECS_VIEW_COMPONENT_HPP

#include <string>

#include <Penrose/ECS/Component.hpp>

namespace Penrose {

    class ViewComponent : public Component {
    public:
        ~ViewComponent() override = default;

        [[nodiscard]] std::string &getRenderList() { return this->_renderList; }

        [[nodiscard]] static constexpr std::string_view name() { return "View"; }

    private:
        std::string _renderList = "Default";
    };
}

#endif // PENROSE_BUILTIN_ECS_VIEW_COMPONENT_HPP
