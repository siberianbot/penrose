#include "RenderListBuilder.hpp"

#include <queue>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Scene/SceneManager.hpp>

#include <Penrose/Builtin/ECS/MeshRendererComponent.hpp>
#include <Penrose/Builtin/ECS/ViewComponent.hpp>

namespace Penrose {

    RenderListBuilder::RenderListBuilder(ResourceSet *resources)
            : _ecsManager(resources->get<ECSManager>()),
              _eventQueue(resources->get<EventQueue>()),
              _sceneManager(resources->get<SceneManager>()) {
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

        auto it = this->_renderListViewMap.find(name);

        if (it == this->_renderListViewMap.end()) {
            return std::nullopt;
        }

        auto viewEntity = it->second;
        View view;

        this->processView(viewEntity, &view);

        auto maybeDrawables = this->discoverDrawables(viewEntity);
        if (!maybeDrawables.has_value()) {
            return std::nullopt;
        }

        return RenderList{
                .view = view,
                .drawables = *maybeDrawables
        };
    }

    void RenderListBuilder::handleComponentCreate(const ComponentEventValue *event) {
        if (event->componentName == MeshRendererComponent::name()) {
            this->_drawables.insert(event->entity);
        }

        if (event->componentName == ViewComponent::name()) {
            auto view = this->_ecsManager->getComponent<ViewComponent>(event->entity);

            this->_renderListViewMap.insert_or_assign(view->getRenderList(), event->entity);
        }
    }

    void RenderListBuilder::handleComponentDestroy(const ComponentEventValue *event) {
        if (event->componentName == MeshRendererComponent::name()) {
            this->_drawables.erase(event->entity);
        }

        if (event->componentName == ViewComponent::name()) {
            auto entity = event->entity;

            auto it = std::find_if(this->_renderListViewMap.begin(), this->_renderListViewMap.end(),
                                   [&entity](const auto &entry) {
                                       return entry.second == entity;
                                   });

            if (it != this->_renderListViewMap.end()) {
                this->_renderListViewMap.erase(it);
            }
        }
    }

    std::optional<std::map<Entity, Drawable>> RenderListBuilder::discoverDrawables(const Entity &viewEntity) const {
        auto maybeNode = this->_sceneManager->tryFindEntityNode(viewEntity);
        if (!maybeNode.has_value()) {
            return std::nullopt;
        }

        auto root = this->_sceneManager->getRoot(*maybeNode);

        std::map<Entity, Drawable> drawables;
        std::queue<SceneNodePtr> queue;
        queue.push(root);

        while (!queue.empty()) {
            auto current = queue.front();
            queue.pop();

            for (const auto &descendant: current->getDescendants()) {
                queue.push(descendant);
            }

            if (current->getEntity().has_value() && this->_drawables.contains(*current->getEntity())) {
                auto [it, _] = drawables.insert_or_assign(*current->getEntity(), Drawable{});

                this->processDrawable(it->first, &it->second);
            }
        }

        return drawables;
    }

    void RenderListBuilder::processDrawable(const Entity &entity, Drawable *drawable) const {
        auto components = this->_ecsManager->queryEntity(entity);

        for (const auto &component: components) {
            auto visitor = dynamic_pointer_cast<DrawableVisitor>(component);

            if (visitor == nullptr) {
                continue;
            }

            visitor->visit(drawable);
        }
    }

    void RenderListBuilder::processView(const Entity &entity, View *view) const {
        auto components = this->_ecsManager->queryEntity(entity);

        for (const auto &component: components) {
            auto visitor = dynamic_pointer_cast<ViewVisitor>(component);

            if (visitor == nullptr) {
                continue;
            }

            visitor->visit(view);
        }
    }
}
