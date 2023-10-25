#include "DefaultDrawableProvider.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <Penrose/Builtin/Penrose/ECS/MeshRendererComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/TransformComponent.hpp>

namespace Penrose {

    DefaultDrawableProvider::DefaultDrawableProvider(ResourceSet *resources)
            : _entityManager(resources->get<EntityManager>()) {
        //
    }

    std::vector<Drawable> DefaultDrawableProvider::getDrawablesFor(const Entity &entity) {
        auto maybeMeshRenderer = this->_entityManager->tryGetComponent<MeshRendererComponent>(entity);

        if (!maybeMeshRenderer.has_value()) {
            return {};
        }

        auto meshRenderer = *maybeMeshRenderer;

        if (!meshRenderer->getMeshAsset().has_value() || !meshRenderer->getAlbedoTextureAsset().has_value()) {
            return {};
        }

        auto drawable = Drawable{
                .entity = entity,
                .meshAsset = *meshRenderer->getMeshAsset(),
                .albedoTextureAsset = *meshRenderer->getAlbedoTextureAsset(),
                .color = meshRenderer->getColor()
        };

        auto maybeTransform = this->_entityManager->tryGetComponent<TransformComponent>(entity);

        if (maybeTransform.has_value()) {
            auto transform = *maybeTransform;

            auto pos = glm::translate(glm::mat4(1), transform->getPos());
            auto rot = glm::rotate(glm::mat4(1), transform->getRot().y, glm::vec3(0, 1, 0)) *
                       glm::rotate(glm::mat4(1), transform->getRot().x, glm::vec3(1, 0, 0)) *
                       glm::rotate(glm::mat4(1), transform->getRot().z, glm::vec3(0, 0, 1));
            auto scale = glm::scale(glm::mat4(1), transform->getScale());

            drawable.model = pos * rot * scale;
            drawable.modelRot = rot;
        }

        return {drawable};
    }
}
