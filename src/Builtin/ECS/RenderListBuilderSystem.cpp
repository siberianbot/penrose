#include "RenderListBuilderSystem.hpp"

#include <ranges>
#include <queue>
#include <set>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Rendering/RenderContext.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Scene/SceneManager.hpp>

#include <Penrose/Builtin/ECS/MeshRendererComponent.hpp>
#include <Penrose/Builtin/ECS/RenderSourceComponent.hpp>

namespace Penrose {

    RenderListBuilderSystem::RenderListBuilderSystem(ResourceSet *resources)
            : _ecsManager(resources->get<ECSManager>()),
              _eventQueue(resources->get<EventQueue>()),
              _renderContext(resources->get<RenderContext>()),
              _sceneManager(resources->get<SceneManager>()) {
        //
    }

    void RenderListBuilderSystem::init() {
        auto sources = this->_ecsManager->queryComponents<RenderSourceComponent>();
        this->_sources.insert(sources.begin(), sources.end());

        this->_eventHandlerIdx = this->_eventQueue->addHandler([this](const Event &event) {
            auto value = event.tryGetComponentEvent();

            if (!value.has_value()) {
                return;
            }

            switch (event.type) {
                case EventType::ComponentCreated:
                    this->handleComponentCreation(*value);
                    break;

                case EventType::ComponentDestroyed:
                    this->handleComponentDestruction(*value);
                    break;

                default:
                    throw EngineError("Not supported component event");
            }
        });
    }

    void RenderListBuilderSystem::destroy() {
        this->_eventQueue->removeHandler(this->_eventHandlerIdx);
        this->_sources.clear();
    }

    void RenderListBuilderSystem::update(float) {
        if (this->_sources.empty()) {
            return;
        }

        for (const auto &source: this->_sources) {
            auto renderSource = this->_ecsManager->getComponent<RenderSourceComponent>(source);
            auto renderList = this->_renderContext->tryGetRenderList(renderSource->getRenderList())
                    .value_or(RenderList{});

            auto lock = this->_renderContext->acquireContextLock();

            auto sceneDrawables = this->discoverDrawables(source);
            if (!sceneDrawables.has_value()) {
                continue;
            }

            auto removedDrawablesView = renderList.drawables |
                                        std::views::keys |
                                        std::views::filter([&sceneDrawables](const Entity &drawable) {
                                            return !sceneDrawables->contains(drawable);
                                        });

            std::set<Entity> removedDrawables;
            for (const auto &drawable: removedDrawablesView) {
                removedDrawables.insert(drawable);
            }

            for (const auto &drawable: removedDrawables) {
                renderList.drawables.erase(drawable);
            }

            for (const auto &drawable: *sceneDrawables) {
                this->processDrawable(drawable, &renderList.drawables[drawable]);
            }

            this->processSource(source, &renderList.view);

            this->_renderContext->setRenderList(renderSource->getRenderList(), renderList);
        }
    }

    void RenderListBuilderSystem::handleComponentCreation(const ComponentEventValue *value) {
        if (value->componentName == RenderSourceComponent::name()) {
            this->_sources.insert(value->entity);
        }

        if (value->componentName == MeshRendererComponent::name()) {
            this->_drawables.insert(value->entity);
        }
    }

    void RenderListBuilderSystem::handleComponentDestruction(const ComponentEventValue *value) {
        if (value->componentName == RenderSourceComponent::name()) {
            this->_sources.erase(value->entity);
        }

        if (value->componentName == MeshRendererComponent::name()) {
            this->_drawables.erase(value->entity);
        }
    }

    std::optional<std::set<Entity>> RenderListBuilderSystem::discoverDrawables(Entity sourceEntity) const {
        auto maybeScene = this->_sceneManager->getCurrentScene();
        if (!maybeScene.has_value()) {
            return std::nullopt;
        }

        auto maybeSceneNode = maybeScene->findEntityNode(sourceEntity);
        if (!maybeSceneNode.has_value()) {
            return std::nullopt;
        }

        auto root = maybeSceneNode.value();
        while (root != nullptr) {
            if (!root->parent.has_value()) {
                break;
            }

            if (root->parent->expired()) {
                break;
            }

            root = root->parent->lock();
        }

        std::set<Entity> entities;
        std::set<std::shared_ptr<SceneTreeNode>> discovered;
        std::queue<std::shared_ptr<SceneTreeNode>> queue;
        queue.push(root);

        while (!queue.empty()) {
            auto current = queue.front();
            queue.pop();

            for (const auto &descendant: current->descendants) {
                if (discovered.contains(descendant)) {
                    continue;
                }

                queue.push(descendant);
            }

            if (current->entity.has_value() && this->_drawables.contains(*current->entity)) {
                entities.insert(*current->entity);
            }

            discovered.insert(current);
        }

        return entities;
    }

    void RenderListBuilderSystem::processSource(Entity entity, View *view) const {
        auto components = this->_ecsManager->queryEntity(entity);

        for (const auto &component: components) {
            auto visitor = dynamic_pointer_cast<ViewVisitor>(component);

            if (visitor == nullptr) {
                continue;
            }

            visitor->visit(view);
        }
    }

    void RenderListBuilderSystem::processDrawable(Entity entity, Drawable *drawable) const {
        auto components = this->_ecsManager->queryEntity(entity);

        for (const auto &component: components) {
            auto visitor = dynamic_pointer_cast<DrawableVisitor>(component);

            if (visitor == nullptr) {
                continue;
            }

            visitor->visit(drawable);
        }
    }
}
