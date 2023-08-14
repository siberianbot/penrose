#include <Penrose/Assets/MeshAsset.hpp>

namespace Penrose {

    MeshAsset::MeshAsset(std::unique_ptr<Buffer> vertexBuffer,
                         std::unique_ptr<Buffer> indexBuffer)
            : _vertexBuffer(std::move(vertexBuffer)),
              _indexBuffer(std::move(indexBuffer)) {
        //
    }
}
