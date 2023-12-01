#include "MeshLoader.hpp"

#include <Penrose/Assets/MeshAsset.hpp>
#include <Penrose/Common/Vertex.hpp>

#include "src/Assets/Structs.hpp"

namespace Penrose {

    MeshLoader::MeshLoader(ResourceSet *resources)
        : _renderingObjectManager(resources->get<RenderingObjectManager>()) {
        //
    }

    Asset *MeshLoader::fromReader(AssetReader &reader) {
        const auto [verticesCount, indicesCount] = reader.read<MeshInfo>();

        const auto verticesSize = sizeof(Vertex) * verticesCount;
        const auto indicesSize = sizeof(std::uint32_t) * indicesCount;

        auto vertices = std::vector<Vertex>(verticesCount);
        auto indices = std::vector<std::uint32_t>(indicesCount);

        reader.read(verticesSize, vertices.data());
        reader.read(indicesSize, indices.data());

        const auto vertexBuffer = this->_renderingObjectManager->makeBuffer(
            BufferType::Vertex, verticesSize, verticesCount, vertices.data()
        );

        const auto indexBuffer = this->_renderingObjectManager->makeBuffer(
            BufferType::Index, indicesSize, indicesCount, indices.data()
        );

        return new MeshAsset(std::shared_ptr<Buffer>(vertexBuffer), std::shared_ptr<Buffer>(indexBuffer));
    }
}
