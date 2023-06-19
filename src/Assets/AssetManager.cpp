#include "AssetManager.hpp"

#include <vector>
#include <string>
#include <tuple>

#include <fmt/core.h>
#include <vulkan/vulkan.hpp>
#include <tiny_obj_loader.h>

#include "src/Assets/AssetDictionary.hpp"
#include "src/Common/EngineError.hpp"
#include "src/Common/Vertex.hpp"
#include "src/Rendering/DeviceContext.hpp"
#include "src/Resources/ResourceSet.hpp"

namespace Penrose {

    AssetManager::AssetManager(ResourceSet *resources)
            : _assetDictionary(resources->get<AssetDictionary>()),
              _deviceContext(resources->get<DeviceContext>()) {
        //
    }

    ShaderAsset AssetManager::loadShader(const AssetId &asset) const {
        auto openedAsset = this->_assetDictionary->openAsset(asset);
        auto createInfo = vk::ShaderModuleCreateInfo()
                .setPCode(reinterpret_cast<const std::uint32_t *>(openedAsset.data()))
                .setCodeSize(openedAsset.size());

        try {
            return ShaderAsset{
                    .shaderModule = makeShaderModule(this->_deviceContext, createInfo)
            };
        } catch (...) {
            std::throw_with_nested(EngineError(fmt::format("Failed to create shader module from asset {}", asset)));
        }
    }

    MeshAsset AssetManager::loadMesh(const AssetId &asset) const {
        auto openedAsset = this->_assetDictionary->openAsset(asset);
        auto assetText = std::string(reinterpret_cast<const char *>(openedAsset.data()), openedAsset.size());

        auto reader = tinyobj::ObjReader();
        if (!reader.ParseFromString(assetText, "") || !reader.Valid()) {
            throw EngineError(fmt::format("Failed to read mesh from asset {}: {}", asset, reader.Error()));
        }

        // TODO: pass warning

        auto &attrib = reader.GetAttrib();
        auto &shapes = reader.GetShapes();
        std::vector<Vertex> vertices;
        std::vector<std::uint32_t> indices;

        for (const auto &shape: shapes) {
            for (const auto &index: shape.mesh.indices) {
                indices.push_back(vertices.size());

                auto vertex = Vertex{
                        .pos = {
                                attrib.vertices.at(3 * index.vertex_index + 0),
                                attrib.vertices.at(3 * index.vertex_index + 1),
                                attrib.vertices.at(3 * index.vertex_index + 2)
                        }
                };

                if (index.normal_index >= 0) {
                    vertex.normal = {
                            attrib.normals.at(3 * index.normal_index + 0),
                            attrib.normals.at(3 * index.normal_index + 1),
                            attrib.normals.at(3 * index.normal_index + 2)
                    };
                }

                if (index.texcoord_index >= 0) {
                    vertex.uv = {
                            attrib.texcoords.at(2 * index.texcoord_index + 0),
                            attrib.texcoords.at(2 * index.texcoord_index + 1)
                    };
                }

                if (!attrib.colors.empty()) {
                    vertex.color = {
                            attrib.colors.at(3 * index.vertex_index + 0),
                            attrib.colors.at(3 * index.vertex_index + 1),
                            attrib.colors.at(3 * index.vertex_index + 2)
                    };
                }

                vertices.push_back(vertex);
            }
        }

        auto makeStaging = [this](vk::DeviceSize size, void *data) {
            auto buffer = makeBuffer(this->_deviceContext, vk::BufferCreateInfo()
                    .setUsage(vk::BufferUsageFlagBits::eTransferSrc)
                    .setSize(size));
            auto memory = makeDeviceMemory(this->_deviceContext, buffer, false);

            void *bufferData;
            auto result = this->_deviceContext->getLogicalDevice().mapMemory(memory.getInstance(), 0, size,
                                                                             vk::MemoryMapFlags(), &bufferData);

            if (result != vk::Result::eSuccess) {
                throw EngineError("Failed to map buffer's memory");
            }

            std::memcpy(bufferData, data, size);
            this->_deviceContext->getLogicalDevice().unmapMemory(memory.getInstance());

            return std::make_tuple(std::move(buffer), std::move(memory));
        };

        auto makeTarget = [this](vk::DeviceSize size, vk::BufferUsageFlags additionalUsage) {
            auto buffer = makeBuffer(this->_deviceContext, vk::BufferCreateInfo()
                    .setUsage(vk::BufferUsageFlagBits::eTransferDst | additionalUsage)
                    .setSize(size));
            auto memory = makeDeviceMemory(this->_deviceContext, buffer);

            return std::make_tuple(std::move(buffer), std::move(memory));
        };

        auto vertexBufferSize = sizeof(Vertex) * vertices.size();
        auto indexBufferSize = sizeof(std::uint32_t) * indices.size();
        auto [stagingVertexBuffer, stagingVertexBufferMemory] = makeStaging(vertexBufferSize, vertices.data());
        auto [stagingIndexBuffer, stagingIndexBufferMemory] = makeStaging(indexBufferSize, indices.data());
        auto [targetVertexBuffer, targetVertexBufferMemory] = makeTarget(vertexBufferSize,
                                                                         vk::BufferUsageFlagBits::eVertexBuffer);
        auto [targetIndexBuffer, targetIndexBufferMemory] = makeTarget(vertexBufferSize,
                                                                       vk::BufferUsageFlagBits::eIndexBuffer);

        auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo()
                .setCommandBufferCount(1)
                .setCommandPool(this->_deviceContext->getCommandPool());
        auto commandBuffers = this->_deviceContext->getLogicalDevice()
                .allocateCommandBuffers(commandBufferAllocateInfo);

        auto beginInfo = vk::CommandBufferBeginInfo()
                .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
        commandBuffers.at(0).begin(beginInfo);
        commandBuffers.at(0).copyBuffer(stagingVertexBuffer.getInstance(),
                                        targetVertexBuffer.getInstance(),
                                        vk::BufferCopy(0, 0, vertexBufferSize));
        commandBuffers.at(0).copyBuffer(stagingIndexBuffer.getInstance(),
                                        targetIndexBuffer.getInstance(),
                                        vk::BufferCopy(0, 0, indexBufferSize));
        commandBuffers.at(0).end();

        this->_deviceContext->getGraphicsQueue().submit(vk::SubmitInfo().setCommandBuffers(commandBuffers));
        this->_deviceContext->getGraphicsQueue().waitIdle();

        this->_deviceContext->getLogicalDevice().free(this->_deviceContext->getCommandPool(), commandBuffers);

        return MeshAsset{
                .vertexBuffer = std::move(targetVertexBuffer),
                .vertexBufferMemory = std::move(targetVertexBufferMemory),
                .indexBuffer = std::move(targetIndexBuffer),
                .indexBufferMemory = std::move(targetIndexBufferMemory),
                .indexCount = static_cast<std::uint32_t>(indices.size())
        };
    }
}
