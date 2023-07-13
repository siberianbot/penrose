#ifndef PENROSE_BUILTIN_ECS_RENDER_LIST_PROVIDER_COMPONENT_HPP
#define PENROSE_BUILTIN_ECS_RENDER_LIST_PROVIDER_COMPONENT_HPP

#include <string>

#include <Penrose/ECS/Component.hpp>

namespace Penrose {

    class RenderListProviderComponent : public Component {
    public:
        ~RenderListProviderComponent() override = default;

        [[nodiscard]] std::string &getRenderList() { return this->_renderList; }

        [[nodiscard]] static constexpr std::string_view name() { return "RenderListProvider"; }

    private:
        std::string _renderList = "Default";
    };
}

#endif // PENROSE_BUILTIN_ECS_RENDER_LIST_PROVIDER_COMPONENT_HPP
