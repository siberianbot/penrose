#include "RenderListProvider.hpp"

#include <stack>
#include <unordered_set>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "src/ECS/ECSManager.hpp"
#include "src/ECS/Components/MeshRendererComponent.hpp"
#include "src/ECS/Components/TransformComponent.hpp"
#include "src/Resources/ResourceSet.hpp"
#include "src/Scene/SceneManager.hpp"
#include "src/Utils/OptionalUtils.hpp"

namespace Penrose {

    std::optional<RenderListItem> RenderListProvider::buildRenderListItem(const Entity &entity) const {
        auto transformComponent = map(this->_ecsManager->tryGetComponent<TransformComponent>(entity),
                                      [](const std::weak_ptr<TransformComponent> &component) {
                                          return component.lock();
                                      });
        auto meshRendererComponent = map(this->_ecsManager->tryGetComponent<MeshRendererComponent>(entity),
                                         [](const std::weak_ptr<MeshRendererComponent> &component) {
                                             return component.lock();
                                         });

        if (!transformComponent.has_value() || !meshRendererComponent.has_value()) {
            return std::nullopt;
        }

        if (!meshRendererComponent.value()->getMesh().has_value()) {
            return std::nullopt;
        }

        auto modelPos = glm::translate(glm::mat4(1), transformComponent.value()->getPos());
        auto modelRot = glm::mat4_cast(glm::quat(transformComponent.value()->getRot()));
        auto modelScale = glm::scale(glm::mat4(1), transformComponent.value()->getScale());
        auto modelMatrix = modelPos * modelRot * modelScale;

        // TODO: fetch aspect ratio
        auto camProjection = glm::perspective(glm::radians(45.0f), 1.0f, 0.0001f, 100.0f);
        camProjection[1][1] *= -1;
        auto camView = glm::lookAt(glm::vec3(4), glm::vec3(0), glm::vec3(0, 1, 0));

        auto matrix = camProjection * camView * modelMatrix;

        auto item = RenderListItem{
                .mesh = *meshRendererComponent.value()->getMesh(),
                .data = RenderData{
                        .matrix = matrix,
                        .model = ModelData{
                                .matrix = modelMatrix,
                                .rotOnly = modelRot
                        }
                }
        };

        return item;
    }

    RenderListProvider::RenderListProvider(Penrose::ResourceSet *resources)
            : _ecsManager(resources->get<ECSManager>()),
              _sceneManager(resources->get<SceneManager>()) {
        //
    }

    std::optional<RenderList> RenderListProvider::buildRenderList() const {
        auto currentScene = this->_sceneManager->getCurrentScene();

        if (!currentScene.has_value()) {
            return std::nullopt;
        }

        RenderList renderList;
        std::unordered_set<std::shared_ptr<SceneTreeNode>> discovered;
        std::stack<std::shared_ptr<SceneTreeNode>> stack;

        stack.push(currentScene->getRoot());

        while (!stack.empty()) {
            auto current = stack.top();
            stack.pop();

            if (!discovered.contains(current)) {
                if (current->entity.has_value()) {
                    auto builtItem = this->buildRenderListItem(*current->entity);

                    if (builtItem.has_value()) {
                        renderList.items.push_back(*builtItem);
                    }
                }

                for (const auto &descendant: current->descendants) {
                    stack.push(descendant);
                }

                discovered.insert(current);
            }
        }

        return renderList;
    }
}
