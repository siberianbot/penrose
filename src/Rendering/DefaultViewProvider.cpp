#include "DefaultViewProvider.hpp"

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Penrose/Builtin/Penrose/ECS/OrthographicCameraComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/PerspectiveCameraComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/TransformComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/ViewComponent.hpp>

namespace Penrose {

    DefaultViewProvider::DefaultViewProvider(ResourceSet *resources)
            : _entityManager(resources->get<EntityManager>()) {
        //
    }

    std::optional<View> DefaultViewProvider::tryGetViewFor(const Entity &entity) {

        if (!this->_entityManager->hasComponent<ViewComponent>(entity)) {
            return std::nullopt;
        }

        View view;

        auto maybeTransform = this->_entityManager->tryGetComponent<TransformComponent>(entity);

        if (maybeTransform.has_value()) {
            auto transform = *maybeTransform;

            auto rotation = glm::rotate(glm::mat4(1), transform->getRot().y, glm::vec3(0, 1, 0)) *
                            glm::rotate(glm::mat4(1), transform->getRot().x, glm::vec3(1, 0, 0)) *
                            glm::rotate(glm::mat4(1), transform->getRot().z, glm::vec3(0, 0, 1));

            auto forward = glm::vec3(rotation * glm::vec4(1, 0, 0, 1));
            auto up = glm::vec3(rotation * glm::vec4(0, 1, 0, 1));

            view.view = glm::lookAt(transform->getPos(), transform->getPos() + forward, up);
        }

        auto maybeOrthographicCamera = this->_entityManager->tryGetComponent<OrthographicCameraComponent>(entity);

        if (maybeOrthographicCamera.has_value()) {
            auto orthographicCamera = *maybeOrthographicCamera;

            view.projection = OrthographicProjection{
                    orthographicCamera->getTop(),
                    orthographicCamera->getBottom(),
                    orthographicCamera->getLeft(),
                    orthographicCamera->getRight(),
                    orthographicCamera->getNear(),
                    orthographicCamera->getFar()
            };
        }

        auto maybePerspectiveCamera = this->_entityManager->tryGetComponent<PerspectiveCameraComponent>(entity);

        if (maybePerspectiveCamera.has_value()) {
            auto perspectiveCamera = *maybePerspectiveCamera;

            view.projection = PerspectiveProjection{
                    perspectiveCamera->getFov(),
                    perspectiveCamera->getNear(),
                    perspectiveCamera->getFar()
            };
        }

        return view;
    }
}
