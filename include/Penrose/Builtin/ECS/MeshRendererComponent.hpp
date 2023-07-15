#ifndef PENROSE_BUILTIN_ECS_MESH_RENDERER_COMPONENT_HPP
#define PENROSE_BUILTIN_ECS_MESH_RENDERER_COMPONENT_HPP

#include <optional>
#include <string>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/Rendering/RenderList.hpp>

namespace Penrose {

    class MeshRendererComponent : public Component, public DrawableVisitor {
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

        void visit(Drawable *drawable) const override {
            drawable->meshAsset = this->_meshAsset;
            drawable->albedoTextureAsset = this->_albedoTextureAsset;
        }

        [[nodiscard]] static constexpr std::string_view name() { return "MeshRenderer"; }

    private:
        std::optional<std::string> _meshAsset = std::nullopt;
        std::optional<std::string> _albedoTextureAsset = std::nullopt;
    };
}

#endif // PENROSE_BUILTIN_ECS_MESH_RENDERER_COMPONENT_HPP
