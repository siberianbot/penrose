#ifndef PENROSE_BUILTIN_ECS_MESH_RENDERER_COMPONENT_HPP
#define PENROSE_BUILTIN_ECS_MESH_RENDERER_COMPONENT_HPP

#include <optional>

#include <Penrose/Assets/AssetId.hpp>
#include <Penrose/ECS/Component.hpp>
#include <Penrose/ECS/ECSBase.hpp>

namespace Penrose {

    class MeshRendererComponent : public Component {
    private:
        std::optional<AssetId> _mesh = std::nullopt;

    public:
        ~MeshRendererComponent() override = default;

        [[nodiscard]] const std::optional<AssetId> &getMesh() const { return this->_mesh; }

        void setMesh(const std::optional<AssetId> &mesh) {
            this->_mesh = mesh;
            this->_dirty = true;
        }

        [[nodiscard]] static constexpr ComponentName name() { return "MeshRenderer"; }
    };
}

#endif // PENROSE_BUILTIN_ECS_MESH_RENDERER_COMPONENT_HPP
