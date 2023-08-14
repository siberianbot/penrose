#ifndef PENROSE_ASSETS_MESH_ASSET_FACTORY_HPP
#define PENROSE_ASSETS_MESH_ASSET_FACTORY_HPP

#include <cstdint>
#include <vector>

#include <Penrose/Assets/MeshAsset.hpp>
#include <Penrose/Common/Vertex.hpp>
#include <Penrose/Resources/Resource.hpp>

namespace Penrose {

    class MeshAssetFactory : public Resource {
    public:
        ~MeshAssetFactory() override = default;

        [[nodiscard]] virtual MeshAsset *makeMesh(std::vector<Vertex> &&vertices,
                                                  std::vector<std::uint32_t> &&indices) = 0;
    };
}

#endif // PENROSE_ASSETS_MESH_ASSET_FACTORY_HPP
