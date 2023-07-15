#ifndef PENROSE_BUILTIN_ECS_RENDER_LIST_BUILDER_SYSTEM_HPP
#define PENROSE_BUILTIN_ECS_RENDER_LIST_BUILDER_SYSTEM_HPP

#include <optional>
#include <unordered_set>

#include <Penrose/ECS/Entity.hpp>
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
    public:
        explicit RenderListBuilderSystem(ResourceSet *resources);
        ~RenderListBuilderSystem() override = default;

        void init() override;
        void destroy() override;

        void update(float) override;

        [[nodiscard]] constexpr static std::string_view name() { return "RenderListBuilder"; }

    private:
        ECSManager *_ecsManager;
        EventQueue *_eventQueue;
        RenderContext *_renderContext;
        SceneManager *_sceneManager;

        EventHandlerIndex _eventHandlerIdx = -1;
        std::unordered_set<Entity> _sources;
        std::unordered_set<Entity> _drawables;

        void handleComponentCreation(const ComponentEventValue *value);
        void handleComponentDestruction(const ComponentEventValue *value);

        [[nodiscard]] std::optional<std::set<Entity>> discoverDrawables(Entity sourceEntity) const;

        void processSource(Entity entity, View *view) const;
        void processDrawable(Entity entity, Drawable *drawable) const;
    };
}

#endif // PENROSE_BUILTIN_ECS_RENDER_LIST_BUILDER_SYSTEM_HPP
