#include "DefaultViewProvider.hpp"

#include <ranges>

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>
#include <Penrose/Utils/RangesUtils.hpp>

#include <Penrose/Builtin/Penrose/ECS/OrthographicCameraComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/PerspectiveCameraComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/TransformComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/ViewComponent.hpp>

namespace Penrose {

    DefaultViewProvider::DefaultViewProvider(ResourceSet *resources)
            : _ecsManager(resources->getLazy<ECSManager>()) {
        //
    }

    std::optional<View> DefaultViewProvider::tryGetViewFor(const Entity &entity) {
        auto ecsQuery = ECSQuery().entity(entity);
        auto components = toMap<std::string, std::shared_ptr<Component>>(
                this->_ecsManager->query(ecsQuery)
                | std::views::transform([](const ECSEntry &entry) {
                    return std::make_pair(entry.componentName, entry.component);
                })
        );

        if (!components.contains(ViewComponent::name())) {
            return std::nullopt;
        }

        View view;

        auto maybeTransform = map(
                tryGet(components, TransformComponent::name()),
                [](const std::shared_ptr<Component> &component) {
                    return std::dynamic_pointer_cast<TransformComponent>(component);
                });

        if (maybeTransform.has_value()) {
            auto transform = *maybeTransform;

            auto rotation = glm::rotate(glm::mat4(1), transform->getRot().y, glm::vec3(0, 1, 0)) *
                            glm::rotate(glm::mat4(1), transform->getRot().x, glm::vec3(1, 0, 0)) *
                            glm::rotate(glm::mat4(1), transform->getRot().z, glm::vec3(0, 0, 1));

            auto forward = glm::vec3(rotation * glm::vec4(1, 0, 0, 1));
            auto up = glm::vec3(rotation * glm::vec4(0, 1, 0, 1));

            view.view = glm::lookAt(transform->getPos(), transform->getPos() + forward, up);
        }

        auto maybeOrthographicCamera = map(
                tryGet(components, OrthographicCameraComponent::name()),
                [](const std::shared_ptr<Component> &component) {
                    return std::dynamic_pointer_cast<OrthographicCameraComponent>(component);
                });

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

        auto maybePerspectiveCamera = map(
                tryGet(components, PerspectiveCameraComponent::name()),
                [](const std::shared_ptr<Component> &component) {
                    return std::dynamic_pointer_cast<PerspectiveCameraComponent>(component);
                });

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
