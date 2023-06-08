#include "MeshRendererComponent.hpp"

namespace Penrose {

    void MeshRendererComponent::setMesh(const std::optional<Asset> &mesh) {
        this->_mesh = mesh;
        this->_dirty = true;
    }

    void MeshRendererComponent::resetDirty() {
        this->_dirty = false;
    }
}
