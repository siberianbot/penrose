#include "DefaultDrawableProvider.hpp"

#include <ranges>

#include <glm/gtc/quaternion.hpp>

#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>
#include <Penrose/Utils/RangesUtils.hpp>

#include <Penrose/Builtin/Penrose/ECS/MeshRendererComponent.hpp>
#include <Penrose/Builtin/Penrose/ECS/TransformComponent.hpp>

namespace Penrose {

    DefaultDrawableProvider::DefaultDrawableProvider(ResourceSet *resources)
            : _ecsManager(resources->getLazy<ECSManager>()) {
        //
    }

    std::vector<Drawable> DefaultDrawableProvider::getDrawablesFor(const Entity &entity) {
        auto ecsQuery = ECSQuery().entity(entity);
        auto components = toMap<std::string, std::shared_ptr<Component>>(
                this->_ecsManager->query(ecsQuery)
                | std::views::transform([](const ECSEntry &entry) {
                    return std::make_pair(entry.componentName, entry.component);
                })
        );

        auto maybeMeshRenderer = map(
                tryGet(components, MeshRendererComponent::name()),
                [](const std::shared_ptr<Component> &component) {
                    return std::dynamic_pointer_cast<MeshRendererComponent>(component);
                });

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
                .albedoTextureAsset = *meshRenderer->getAlbedoTextureAsset()
        };

        auto maybeTransform = map(
                tryGet(components, TransformComponent::name()),
                [](const std::shared_ptr<Component> &component) {
                    return std::dynamic_pointer_cast<TransformComponent>(component);
                });

        if (maybeTransform.has_value()) {
            auto transform = *maybeTransform;

            auto pos = glm::translate(glm::mat4(1), transform->getPos());
            auto rot = glm::mat4_cast(glm::quat(transform->getRot()));
            auto scale = glm::scale(glm::mat4(1), transform->getScale());

            drawable.model = pos * rot * scale;
            drawable.modelRot = rot;
        }

        return {drawable};
    }
}
