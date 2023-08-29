#ifndef PENROSE_RENDERING_RENDER_LIST_BUILDER_HPP
#define PENROSE_RENDERING_RENDER_LIST_BUILDER_HPP

#include <map>
#include <mutex>
#include <optional>
#include <set>
#include <string>

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Events/Event.hpp>
#include <Penrose/Rendering/RenderList.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class ResourceSet;
    class ECSManager;
    class EventQueue;
    class SceneManager;

    class RenderListBuilder : public Resource, public Initializable {
    public:
        explicit RenderListBuilder(ResourceSet *resources);
        ~RenderListBuilder() override = default;

        void init() override;
        void destroy() override;

        [[nodiscard]] std::optional<RenderList> tryBuildRenderList(const std::string &name);

    private:
        ECSManager *_ecsManager;
        EventQueue *_eventQueue;
        SceneManager *_sceneManager;

        EventHandlerIndex _eventHandlerIdx = -1;
        std::mutex _mutex;

        std::set<Entity> _drawables;
        std::map<std::string, Entity> _renderListViewMap;

        void handleComponentCreate(const ComponentEventValue *event);
        void handleComponentDestroy(const ComponentEventValue *event);

        [[nodiscard]] std::optional<std::map<Entity, Drawable>> discoverDrawables(const Entity &viewEntity) const;
        void processDrawable(const Entity &entity, Drawable *drawable) const;
        void processView(const Entity &entity, View *view) const;
    };
}

#endif // PENROSE_RENDERING_RENDER_LIST_BUILDER_HPP
