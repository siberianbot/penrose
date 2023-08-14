#ifndef PENROSE_ASSETS_MESH_ASSET_HPP
#define PENROSE_ASSETS_MESH_ASSET_HPP

#include <memory>

#include <Penrose/Assets/Asset.hpp>
#include <Penrose/Rendering/Buffer.hpp>

namespace Penrose {

    class MeshAsset : public Asset {
    public:
        MeshAsset(std::unique_ptr<Buffer> vertexBuffer,
                  std::unique_ptr<Buffer> indexBuffer);
        ~MeshAsset() override = default;

        [[nodiscard]] AssetType getType() const override { return AssetType::Mesh; }

        [[nodiscard]] Buffer *getVertexBuffer() const { return this->_vertexBuffer.get(); }

        [[nodiscard]] Buffer *getIndexBuffer() const { return this->_indexBuffer.get(); }

    private:
        std::unique_ptr<Buffer> _vertexBuffer;
        std::unique_ptr<Buffer> _indexBuffer;
    };
}

#endif // PENROSE_ASSETS_MESH_ASSET_HPP
