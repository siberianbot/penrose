#ifndef PENROSE_BUILTIN_ECS_MESH_RENDERER_COMPONENT_HPP
#define PENROSE_BUILTIN_ECS_MESH_RENDERER_COMPONENT_HPP

#include <optional>
#include <string>

#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ECSBase.hpp>

namespace Penrose {

    class MeshRendererComponent : public Component {
    private:
        std::optional<std::string> _mesh = std::nullopt;
        std::optional<std::string> _albedo = std::nullopt;

    public:
        ~MeshRendererComponent() override = default;

        [[nodiscard]] const std::optional<std::string> &getMesh() const { return this->_mesh; }

        void setMesh(const std::optional<std::string> &mesh) {
            this->_mesh = mesh;
            this->_dirty = true;
        }

        [[nodiscard]] const std::optional<std::string> &getAlbedo() const { return this->_albedo; }

        void setAlbedo(const std::optional<std::string> &albedo) {
            this->_albedo = albedo;
            this->_dirty = true;
        }

        [[nodiscard]] static constexpr ComponentName name() { return "MeshRenderer"; }
    };
}

#endif // PENROSE_BUILTIN_ECS_MESH_RENDERER_COMPONENT_HPP
