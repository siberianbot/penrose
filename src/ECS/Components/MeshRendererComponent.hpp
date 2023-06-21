#ifndef PENROSE_ECS_COMPONENTS_MESH_RENDERER_COMPONENT_HPP
#define PENROSE_ECS_COMPONENTS_MESH_RENDERER_COMPONENT_HPP

#include <optional>

#include "src/Assets/AssetId.hpp"
#include "src/ECS/Components/Component.hpp"

namespace Penrose {

    class MeshRendererComponent : public Component {
    private:
        bool _dirty;
        std::optional<AssetId> _mesh;

    public:
        ~MeshRendererComponent() override = default;

        [[nodiscard]] bool isDirty() const { return this->_dirty; }

        [[nodiscard]] const std::optional<AssetId> &getMesh() const { return this->_mesh; }

        void setMesh(const std::optional<AssetId> &mesh);
        void resetDirty();

        [[nodiscard]] static ComponentName name() { return "MeshRenderer"; }
    };
}

#endif // PENROSE_ECS_COMPONENTS_MESH_RENDERER_COMPONENT_HPP
