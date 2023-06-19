#ifndef PENROSE_RENDERING_RENDER_LIST_PROVIDER_HPP
#define PENROSE_RENDERING_RENDER_LIST_PROVIDER_HPP

#include <optional>

#include "src/ECS/ECSBase.hpp"
#include "src/Resources/Resource.hpp"
#include "src/Rendering/RenderData.hpp"

namespace Penrose {

    class ResourceSet;
    class ECSManager;
    class SceneManager;

    class RenderListProvider : public Resource {
    private:
        ECSManager *_ecsManager;
        SceneManager *_sceneManager;

        [[nodiscard]] std::optional<RenderListItem> buildRenderListItem(const Entity &entity) const;

    public:
        explicit RenderListProvider(ResourceSet *resources);
        ~RenderListProvider() override = default;

        [[nodiscard]] std::optional<RenderList> buildRenderList() const;
    };
}

#endif // PENROSE_RENDERING_RENDER_LIST_PROVIDER_HPP
