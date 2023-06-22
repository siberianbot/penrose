#ifndef PENROSE_ECS_COMPONENTS_MESH_RENDERER_COMPONENT_HPP
#define PENROSE_ECS_COMPONENTS_MESH_RENDERER_COMPONENT_HPP

#include <optional>

#include "src/Assets/AssetId.hpp"
#include "src/ECS/Components/Component.hpp"

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

        [[nodiscard]] static ComponentName name() { return "MeshRenderer"; }
    };
}

#endif // PENROSE_ECS_COMPONENTS_MESH_RENDERER_COMPONENT_HPP
