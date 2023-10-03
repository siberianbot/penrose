#ifndef PENROSE_BUILTIN_PENROSE_ECS_MESH_RENDERER_COMPONENT_HPP
#define PENROSE_BUILTIN_PENROSE_ECS_MESH_RENDERER_COMPONENT_HPP

#include <optional>
#include <string>

#include <glm/vec3.hpp>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ComponentFactory.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class MeshRendererComponent : public Component {
    public:
        ~MeshRendererComponent() override = default;

        [[nodiscard]] const std::optional<std::string> &getMeshAsset() const { return this->_meshAsset; }

        void setMeshAsset(const std::optional<std::string> &mesh) { this->_meshAsset = mesh; }

        [[nodiscard]] const std::optional<std::string> &getAlbedoTextureAsset() const {
            return this->_albedoTextureAsset;
        }

        void setAlbedoTextureAsset(const std::optional<std::string> &albedo) {
            this->_albedoTextureAsset = albedo;
        }

        [[nodiscard]] const glm::vec3 &getColor() const { return this->_color; }

        void setColor(const glm::vec3 &color) { this->_color = color; }

        [[nodiscard]] std::string getName() const override { return name(); }

        [[nodiscard]] constexpr static std::string name() { return "MeshRenderer"; }

    private:
        std::optional<std::string> _meshAsset = std::nullopt;
        std::optional<std::string> _albedoTextureAsset = std::nullopt;
        glm::vec3 _color = glm::vec3(1);
    };

    class MeshRendererComponentFactory : public Resource, public GenericComponentFactory<MeshRendererComponent> {
    public:
        ~MeshRendererComponentFactory() override = default;
    };
}

#endif // PENROSE_BUILTIN_PENROSE_ECS_MESH_RENDERER_COMPONENT_HPP
