#include "MeshRendererComponent.hpp"

namespace Penrose {

    void MeshRendererComponent::setMesh(const std::optional<AssetId> &mesh) {
        this->_mesh = mesh;
        this->_dirty = true;
    }

    void MeshRendererComponent::resetDirty() {
        this->_dirty = false;
    }
}
