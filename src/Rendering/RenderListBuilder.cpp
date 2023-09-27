#include <Penrose/Rendering/RenderListBuilder.hpp>

#include <queue>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

#include <Penrose/Builtin/Penrose/ECS/ViewComponent.hpp>

namespace Penrose {

    RenderListBuilder::RenderListBuilder(ResourceSet *resources)
            : _eventQueue(resources->getLazy<EventQueue>()),
              _sceneManager(resources->getLazy<SceneManager>()),
              _drawableProviders(resources->getAllLazy<DrawableProvider>()),
              _viewProviders(resources->getAllLazy<ViewProvider>()) {
        //
    }

    void RenderListBuilder::init() {
        this->_eventHandlerIdx = this->_eventQueue->addHandler<EventType::ECSEvent, ECSEventArgs>(
                [this](const ECSEvent *event) {
                    auto lock = std::lock_guard<std::mutex>(this->_mutex);

                    switch (event->getArgs().type) {
                        case ECSEventType::ComponentCreated: {
                            this->handleComponentCreate(event->getArgs());
                            break;
                        }

                        case ECSEventType::ComponentDestroyed: {
                            this->handleComponentDestroy(event->getArgs());
                            break;
                        }

                        default:
                            /* nothing to do */
                            break;
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

    void RenderListBuilder::handleComponentCreate(const ECSEventArgs &eventArgs) {
        if (eventArgs.componentName != ViewComponent::name()) {
            return;
        }

        auto view = std::dynamic_pointer_cast<ViewComponent>(eventArgs.component);
        this->_renderListViewMap.insert_or_assign(view->getRenderList(), eventArgs.entity);
    }

    void RenderListBuilder::handleComponentDestroy(const ECSEventArgs &eventArgs) {
        if (eventArgs.componentName != ViewComponent::name()) {
            return;
        }

        auto entity = eventArgs.entity;
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
