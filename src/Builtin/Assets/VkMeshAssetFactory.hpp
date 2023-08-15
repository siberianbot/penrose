#ifndef PENROSE_BUILTIN_ASSETS_VK_MESH_ASSET_FACTORY_HPP
#define PENROSE_BUILTIN_ASSETS_VK_MESH_ASSET_FACTORY_HPP

#include <Penrose/Assets/MeshAssetFactory.hpp>

namespace Penrose {

    class ResourceSet;
    class BufferFactory;
    class DeviceContext;

    class VkMeshAssetFactory : public MeshAssetFactory {
    public:
        explicit VkMeshAssetFactory(ResourceSet *resources);
        ~VkMeshAssetFactory() override = default;

        MeshAsset *makeMesh(std::vector<Vertex> &&vertices,
                            std::vector<std::uint32_t> &&indices) override;

    private:
        BufferFactory *_bufferFactory;
        DeviceContext *_deviceContext;
    };
}

#endif // PENROSE_BUILTIN_ASSETS_VK_MESH_ASSET_FACTORY_HPP