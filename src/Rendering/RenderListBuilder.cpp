#include <Penrose/Rendering/RenderListBuilder.hpp>

#include <queue>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

#include <Penrose/Builtin/Penrose/ECS/ViewComponent.hpp>

namespace Penrose {

    RenderListBuilder::RenderListBuilder(ResourceSet *resources)
            : _ecsManager(resources->getLazy<ECSManager>()),
              _eventQueue(resources->getLazy<EventQueue>()),
              _sceneManager(resources->getLazy<SceneManager>()),
              _drawableProviders(resources->getAllLazy<DrawableProvider>()),
              _viewProviders(resources->getAllLazy<ViewProvider>()) {
        //
    }

    void RenderListBuilder::init() {
        this->_eventHandlerIdx = this->_eventQueue->addHandler([this](const Event &event) {
            auto componentEvent = event.tryGetComponentEvent();

            if (!componentEvent.has_value()) {
                return;
            }

            auto lock = std::lock_guard<std::mutex>(this->_mutex);

            switch (event.type) {
                case EventType::ComponentCreated:
                    this->handleComponentCreate(*componentEvent);
                    break;

                case EventType::ComponentDestroyed:
                    this->handleComponentDestroy(*componentEvent);
                    break;

                default:
                    throw EngineError("Not supported event");
            }
        });
    }

    void RenderListBuilder::destroy() {
        this->_eventQueue->removeHandler(this->_eventHandlerIdx);
    }

    std::optional<RenderList> RenderListBuilder::tryBuildRenderList(const std::string &name) {
        auto lock = std::lock_guard<std::mutex>(this->_mutex);

        auto viewEntity = tryGet(this->_renderListViewMap, name);

        if (!viewEntity.has_value()) {
            return std::nullopt;
        }

        std::optional<View> view;

        for (const auto &viewProvider: this->_viewProviders) {
            view = viewProvider->tryGetViewFor(*viewEntity);

            if (view.has_value()) {
                break;
            }
        }

        if (!view.has_value()) {
            return std::nullopt;
        }

        auto maybeDrawableEntities = this->discoverDrawables(*viewEntity);
        std::list<Drawable> drawables;

        if (!maybeDrawableEntities.has_value()) {
            return std::nullopt;
        }

        for (const auto &entity: (*maybeDrawableEntities)) {
            for (const auto &drawableProvider: this->_drawableProviders) {
                auto resultDrawables = drawableProvider->getDrawablesFor(entity);

                drawables.insert(drawables.end(), resultDrawables.begin(), resultDrawables.end());
            }
        }

        return RenderList{
                .view = *view,
                .drawables = std::forward<decltype(drawables)>(drawables)
        };
    }

    void RenderListBuilder::handleComponentCreate(const ComponentEventValue *event) {
        if (event->componentName != ViewComponent::name()) {
            return;
        }

        auto view = this->_ecsManager->getComponent<ViewComponent>(event->entity);
        this->_renderListViewMap.insert_or_assign(view->getRenderList(), event->entity);
    }

    void RenderListBuilder::handleComponentDestroy(const ComponentEventValue *event) {
        if (event->componentName != ViewComponent::name()) {
            return;
        }

        auto entity = event->entity;
        auto it = std::find_if(this->_renderListViewMap.begin(), this->_renderListViewMap.end(),
                               [&entity](const auto &entry) {
                                   return entry.second == entity;
                               });

        if (it != this->_renderListViewMap.end()) {
            this->_renderListViewMap.erase(it);
        }
    }

    std::optional<std::set<Entity>> RenderListBuilder::discoverDrawables(const Entity &viewEntity) {
        auto maybeNode = this->_sceneManager->tryFindEntityNode(viewEntity);
        if (!maybeNode.has_value()) {
            return std::nullopt;
        }

        auto root = this->_sceneManager->getRoot(*maybeNode);

        std::set<Entity> drawableEntities;
        std::queue<SceneNodePtr> queue;
        queue.push(root);

        while (!queue.empty()) {
            auto current = queue.front();
            queue.pop();

            for (const auto &descendant: current->getDescendants()) {
                queue.push(descendant);
            }

            if (current->getEntity().has_value()) {
                drawableEntities.insert(*current->getEntity());
            }
        }

        return drawableEntities;
    }
}
