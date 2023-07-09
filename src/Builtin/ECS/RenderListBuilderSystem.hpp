#ifndef PENROSE_BUILTIN_ECS_RENDER_LIST_BUILDER_SYSTEM_HPP
#define PENROSE_BUILTIN_ECS_RENDER_LIST_BUILDER_SYSTEM_HPP

#include <memory>
#include <optional>
#include <set>

#include <Penrose/ECS/ECSBase.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Events/Event.hpp>
#include <Penrose/Rendering/RenderList.hpp>

namespace Penrose {

    class ResourceSet;
    class ECSManager;
    class EventQueue;
    class RenderContext;
    class SceneManager;

    class RenderListBuilderSystem : public System {
    private:
        ECSManager *_ecsManager;
        EventQueue *_eventQueue;
        RenderContext *_renderContext;
        SceneManager *_sceneManager;

        EventHandlerIndex _eventHandlerIdx = -1;
        std::vector<Entity> _renderListProviders;

        void handleComponentCreation(const ComponentEventValue *value);
        void handleComponentDestruction(const ComponentEventValue *value);
        std::optional<RenderListItem> toRenderListItem(const Entity &entity);

    public:
        explicit RenderListBuilderSystem(ResourceSet *resources);
        ~RenderListBuilderSystem() override = default;

        void init() override;
        void update() override;
        void destroy() override;
    };
}

#endif // PENROSE_BUILTIN_ECS_RENDER_LIST_BUILDER_SYSTEM_HPP
