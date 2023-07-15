#ifndef PENROSE_BUILTIN_ECS_RENDER_SOURCE_COMPONENT_HPP
#define PENROSE_BUILTIN_ECS_RENDER_SOURCE_COMPONENT_HPP

#include <string>

#include <Penrose/ECS/Component.hpp>

namespace Penrose {

    class RenderSourceComponent : public Component {
    public:
        ~RenderSourceComponent() override = default;

        [[nodiscard]] std::string &getRenderList() { return this->_renderList; }

        [[nodiscard]] static constexpr std::string_view name() { return "RenderSource"; }

    private:
        std::string _renderList = "Default";
    };
}

#endif // PENROSE_BUILTIN_ECS_RENDER_SOURCE_COMPONENT_HPP
