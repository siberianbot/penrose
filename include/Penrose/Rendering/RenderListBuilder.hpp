#ifndef PENROSE_RENDERING_RENDER_LIST_BUILDER_HPP
#define PENROSE_RENDERING_RENDER_LIST_BUILDER_HPP

#include <map>
#include <mutex>
#include <optional>
#include <set>

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/Events/Event.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Rendering/DrawableProvider.hpp>
#include <Penrose/Rendering/RenderList.hpp>
#include <Penrose/Rendering/ViewProvider.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Scene/SceneManager.hpp>

namespace Penrose {

    class ResourceSet;

    class RenderListBuilder : public Resource, public Initializable {
    public:
        explicit RenderListBuilder(ResourceSet *resources);
        ~RenderListBuilder() override = default;

        void init() override;
        void destroy() override;

        [[nodiscard]] std::optional<RenderList> tryBuildRenderList(const std::string &name);

    private:
        Lazy<ECSManager> _ecsManager;
        Lazy<EventQueue> _eventQueue;
        Lazy<SceneManager> _sceneManager;
        LazyCollection<DrawableProvider> _drawableProviders;
        LazyCollection<ViewProvider> _viewProviders;

        EventHandlerIndex _eventHandlerIdx = -1;
        std::mutex _mutex;

        std::map<std::string, Entity> _renderListViewMap;

        void handleComponentCreate(const ComponentEventValue *event);
        void handleComponentDestroy(const ComponentEventValue *event);

        [[nodiscard]] std::optional<std::set<Entity>> discoverDrawables(const Entity &viewEntity);
    };
}

#endif // PENROSE_RENDERING_RENDER_LIST_BUILDER_HPP
