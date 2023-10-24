#ifndef PENROSE_RENDERING_RENDER_LIST_BUILDER_HPP
#define PENROSE_RENDERING_RENDER_LIST_BUILDER_HPP

#include <map>
#include <mutex>
#include <optional>
#include <set>

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Events/ECSEvents.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Rendering/DrawableProvider.hpp>
#include <Penrose/Rendering/RenderList.hpp>
#include <Penrose/Rendering/ViewProvider.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Scene/SceneManager.hpp>

namespace Penrose {

    // TODO: Still requires a lot of reworking
    //
    // 1. building render list every frame is not efficient
    //    -> receive EntityCreated / ComponentCreated / SceneModified events and handle them properly
    // 2. ...

    class RenderListBuilder : public Resource<RenderListBuilder>, public Initializable {
    public:
        explicit RenderListBuilder(ResourceSet *resources);
        ~RenderListBuilder() override = default;

        void init() override;
        void destroy() override;

        [[nodiscard]] std::optional<RenderList> tryBuildRenderList(const std::string &name);

    private:
        ResourceProxy<ECSEventQueue> _eventQueue;
        ResourceProxy<SceneManager> _sceneManager;
        ResourceProxy<DrawableProvider> _drawableProviders;
        ResourceProxy<ViewProvider> _viewProviders;

        std::mutex _mutex;

        std::map<std::string, Entity> _renderListViewMap;

        void handleComponentCreate(const ComponentCreatedEvent *event);
        void handleComponentDestroy(const ComponentDestroyedEvent *event);

        [[nodiscard]] std::optional<std::set<Entity>> discoverDrawables(const Entity &viewEntity);
    };
}

#endif // PENROSE_RENDERING_RENDER_LIST_BUILDER_HPP
