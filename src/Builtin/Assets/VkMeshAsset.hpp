#ifndef PENROSE_BUILTIN_ASSETS_VK_MESH_ASSET_HPP
#define PENROSE_BUILTIN_ASSETS_VK_MESH_ASSET_HPP

#include <cstdint>

#include <vulkan/vulkan.hpp>

#include <Penrose/Assets/Asset.hpp>
#include <Penrose/Common/Vertex.hpp>

namespace Penrose {

    class DeviceContext;

    class VkMeshAsset : public MeshAsset {
    public:
        VkMeshAsset(DeviceContext *deviceContext,
                    vk::Buffer vertexBuffer, vk::DeviceMemory vertexBufferMemory,
                    vk::Buffer indexBuffer, vk::DeviceMemory indexBufferMemory, std::uint32_t indexCount);
        ~VkMeshAsset() override;

        [[nodiscard]] const vk::Buffer &getVertexBuffer() const { return this->_vertexBuffer; }

        [[nodiscard]] const vk::Buffer &getIndexBuffer() const { return this->_indexBuffer; }

        [[nodiscard]] const std::uint32_t &getIndexCount() const { return this->_indexCount; }

    private:
        DeviceContext *_deviceContext;
        vk::Buffer _vertexBuffer;
        vk::DeviceMemory _vertexBufferMemory;
        vk::Buffer _indexBuffer;
        vk::DeviceMemory _indexBufferMemory;
        std::uint32_t _indexCount;
    };

    [[nodiscard]] VkMeshAsset *makeVkMeshAsset(DeviceContext *deviceContext,
                                               std::vector<Vertex> &&vertices,
                                               std::vector<std::uint32_t> &&indices);
}

#endif // PENROSE_BUILTIN_ASSETS_VK_MESH_ASSET_HPP
