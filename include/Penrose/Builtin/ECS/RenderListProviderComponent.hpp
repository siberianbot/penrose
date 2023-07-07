#ifndef PENROSE_BUILTIN_ECS_RENDER_LIST_PROVIDER_COMPONENT_HPP
#define PENROSE_BUILTIN_ECS_RENDER_LIST_PROVIDER_COMPONENT_HPP

#include <string>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ECSBase.hpp>

namespace Penrose {

    class RenderListProviderComponent : public Component {
    private:
        std::string _renderList = "Default";

    public:
        ~RenderListProviderComponent() override = default;

        [[nodiscard]] std::string &getRenderList() { return this->_renderList; }

        [[nodiscard]] static constexpr ComponentName name() { return "RenderListProvider"; }
    };
}

#endif // PENROSE_BUILTIN_ECS_RENDER_LIST_PROVIDER_COMPONENT_HPP
