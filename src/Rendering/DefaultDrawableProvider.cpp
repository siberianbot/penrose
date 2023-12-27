#include "DefaultDrawableProvider.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <Penrose/Builtin/Penrose/ECS/MeshComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/TransformComponent.hpp>

namespace Penrose {

    DefaultDrawableProvider::DefaultDrawableProvider(const ResourceSet *resources)
            : _entityManager(resources->get<EntityManager>()) {
        //
    }

    std::vector<Drawable> DefaultDrawableProvider::getDrawablesFor(const Entity &entity) {
        auto maybeMeshRenderer = this->_entityManager->tryGetComponent<MeshComponent>(entity);

        if (!maybeMeshRenderer.has_value()) {
            return {};
        }

        auto meshRenderer = *maybeMeshRenderer;

        if (!meshRenderer->mesh.has_value() || !meshRenderer->albedo.has_value()) {
            return {};
        }

        auto drawable = Drawable{
                .entity = entity,
                .meshAsset = *meshRenderer->mesh,
                .albedoTextureAsset = *meshRenderer->albedo,
                .color = meshRenderer->color
        };

        auto maybeTransform = this->_entityManager->tryGetComponent<TransformComponent>(entity);

        if (maybeTransform.has_value()) {
            auto transform = *maybeTransform;

            auto pos = glm::translate(glm::mat4(1), transform->pos);
            auto rot = glm::rotate(glm::mat4(1), transform->rot.y, glm::vec3(0, 1, 0)) *
                       glm::rotate(glm::mat4(1), transform->rot.x, glm::vec3(1, 0, 0)) *
                       glm::rotate(glm::mat4(1), transform->rot.z, glm::vec3(0, 0, 1));
            auto scale = glm::scale(glm::mat4(1), transform->scale);

            drawable.model = pos * rot * scale;
            drawable.modelRot = rot;
        }

        return {drawable};
    }
}
