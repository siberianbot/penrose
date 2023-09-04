#ifndef PENROSE_BUILTIN_VULKAN_ASSETS_VK_MESH_ASSET_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_ASSETS_VK_MESH_ASSET_FACTORY_HPP

#include <Penrose/Assets/MeshAssetFactory.hpp>
#include <Penrose/Rendering/BufferFactory.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/Rendering/VkCommandManager.hpp"

namespace Penrose {

    class ResourceSet;

    class VkMeshAssetFactory : public Resource, public MeshAssetFactory {
    public:
        explicit VkMeshAssetFactory(ResourceSet *resources);
        ~VkMeshAssetFactory() override = default;

        [[nodiscard]] MeshAsset *makeMesh(std::vector<Vertex> &&vertices,
                                          std::vector<std::uint32_t> &&indices) override;

    private:
        Lazy<BufferFactory> _bufferFactory;
        Lazy<VkCommandManager> _commandManager;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_ASSETS_VK_MESH_ASSET_FACTORY_HPP
