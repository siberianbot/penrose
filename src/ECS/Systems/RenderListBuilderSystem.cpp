#include "RenderListBuilderSystem.hpp"

#include <array>
#include <queue>
#include <set>
#include <vector>

#include <glm/gtc/quaternion.hpp>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Rendering/RenderContext.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/ECS/ECSManager.hpp"
#include "src/ECS/Components/CameraComponent.hpp"
#include "src/ECS/Components/MeshRendererComponent.hpp"
#include "src/ECS/Components/RenderListProviderComponent.hpp"
#include "src/ECS/Components/TransformComponent.hpp"
#include "src/Events/EventQueue.hpp"
#include "src/Scene/SceneManager.hpp"

namespace Penrose {

    void RenderListBuilderSystem::handleComponentCreation(const ComponentEventValue *value) {
        if (value->componentName != RenderListProviderComponent::name()) {
            return;
        }

        this->_renderListProviders.push_back(value->entity);
    }

    void RenderListBuilderSystem::handleComponentDestruction(const ComponentEventValue *value) {
        if (value->componentName != RenderListProviderComponent::name()) {
            return;
        }

        std::erase(this->_renderListProviders, value->entity);
    }

    std::optional<RenderListItem> RenderListBuilderSystem::toRenderListItem(const Entity &entity) {
        auto maybeMeshRenderer = this->_ecsManager->tryGetComponent<MeshRendererComponent>(entity);

        if (!maybeMeshRenderer.has_value() || !(*maybeMeshRenderer)->getMesh().has_value()) {
            return std::nullopt;
        }

        auto item = RenderListItem{
                .mesh = *(*maybeMeshRenderer)->getMesh()
        };

        if (auto transform = this->_ecsManager->tryGetComponent<TransformComponent>(entity)) {
            auto modelPos = glm::translate(glm::mat4(1), (*transform)->getPos());
            auto modelRot = glm::mat4_cast(glm::quat((*transform)->getRot()));
            auto modelScale = glm::scale(glm::mat4(1), (*transform)->getScale());

            item.model = modelPos * modelRot * modelScale;
            item.modelRot = modelRot;
        } else {
            item.model = glm::mat4(1);
            item.modelRot = glm::mat4(1);
        }

        return item;
    }

    RenderListBuilderSystem::RenderListBuilderSystem(ResourceSet *resources)
            : _ecsManager(resources->get<ECSManager>()),
              _eventQueue(resources->get<EventQueue>()),
              _renderContext(resources->get<RenderContext>()),
              _sceneManager(resources->get<SceneManager>()) {
        //
    }

    void RenderListBuilderSystem::init() {
        this->_eventHandlerIdx = this->_eventQueue->addHandler([this](const Event &event) {
            if (!event.value.has_value()) {
                return;
            }

            const ComponentEventValue *value = std::get_if<ComponentEventValue>(&*event.value);

            if (value == nullptr) {
                return;
            }

            switch (event.type) {
                case EventType::ComponentCreated:
                    this->handleComponentCreation(value);
                    break;

                case EventType::ComponentDestroyed:
                    this->handleComponentDestruction(value);
                    break;

                default:
                    throw EngineError("Not supported component event");
            }
        });
    }

    void RenderListBuilderSystem::update() {
        if (this->_renderListProviders.empty()) {
            return;
        }

        for (const auto &providerEntity: this->_renderListProviders) {
            auto renderListProvider = this->_ecsManager->getComponent<RenderListProviderComponent>(providerEntity);

            auto maybeScene = this->_sceneManager->getCurrentScene();
            if (!maybeScene.has_value()) {
                // TODO: wtf?
                continue;
            }

            auto maybeSceneNode = maybeScene->findEntityNode(providerEntity);
            if (!maybeSceneNode.has_value()) {
                // TODO: wtf?
                continue;
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

                if (current->entity.has_value()) {
                    entities.insert(*current->entity);
                }

                discovered.insert(current);
            }

            auto lock = this->_renderContext->acquireContextLock();
            auto renderList = this->_renderContext->tryGetRenderList(renderListProvider->getRenderList())
                    .value_or(RenderList{});

            std::vector<Entity> removedEntities;
            std::set_difference(renderList.entities.begin(), renderList.entities.end(),
                                entities.begin(), entities.end(),
                                std::inserter(removedEntities, removedEntities.begin()));

            for (const auto &removedEntity: removedEntities) {
                renderList.items.erase(removedEntity);
            }

            for (const auto &entity: entities) {
                if (auto item = this->toRenderListItem(entity)) {
                    renderList.items[entity] = *item;
                } else {
                    renderList.items.erase(entity);
                }
            }

            if (auto camera = this->_ecsManager->tryGetComponent<CameraComponent>(providerEntity)) {
                renderList.projection = Perspective{
                        .fov = (*camera)->getFov()
                };

                renderList.near = (*camera)->getNear();
                renderList.far = (*camera)->getFar();
            } else {
                renderList.near = 0.0001f;
                renderList.far = 100.0f;
            }

            if (auto transform = this->_ecsManager->tryGetComponent<TransformComponent>(providerEntity)) {
                auto pos = (*transform)->getPos();
                auto target = glm::mat3_cast(glm::quat((*transform)->getRot())) * glm::vec3(1, 0, 0);

                renderList.view = glm::lookAt(pos, pos + target, glm::vec3(0, 1, 0));
            } else {
                renderList.view = glm::lookAt(glm::vec3(0), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
            }

            renderList.entities = entities;

            this->_renderContext->setRenderList(renderListProvider->getRenderList(), renderList);
        }
    }

    void RenderListBuilderSystem::destroy() {
        this->_eventQueue->removeHandler(this->_eventHandlerIdx);
    }
}
