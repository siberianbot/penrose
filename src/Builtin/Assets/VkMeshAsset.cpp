#include "VkMeshAsset.hpp"

#include <Penrose/Common/EngineError.hpp>

#include "src/Rendering/DeviceContext.hpp"

#include <src/Builtin/Rendering/VkUtils.hpp>

namespace Penrose {

    VkMeshAsset::VkMeshAsset(DeviceContext *deviceContext,
                             vk::Buffer vertexBuffer, vk::DeviceMemory vertexBufferMemory,
                             vk::Buffer indexBuffer, vk::DeviceMemory indexBufferMemory, std::uint32_t indexCount)
            : _deviceContext(deviceContext),
              _vertexBuffer(vertexBuffer), _vertexBufferMemory(vertexBufferMemory),
              _indexBuffer(indexBuffer), _indexBufferMemory(indexBufferMemory), _indexCount(indexCount) {
        //
    }

    VkMeshAsset::~VkMeshAsset() {
        this->_deviceContext->getLogicalDevice().free(this->_indexBufferMemory);
        this->_deviceContext->getLogicalDevice().destroy(this->_indexBuffer);

        this->_deviceContext->getLogicalDevice().free(this->_vertexBufferMemory);
        this->_deviceContext->getLogicalDevice().destroy(this->_vertexBuffer);
    }

    VkMeshAsset *makeVkMeshAsset(DeviceContext *deviceContext,
                                 std::vector<Vertex> &&vertices,
                                 std::vector<std::uint32_t> &&indices) {
        auto makeStaging = [deviceContext](vk::DeviceSize size, void *data) {
            auto bufferCreateInfo = vk::BufferCreateInfo()
                    .setUsage(vk::BufferUsageFlagBits::eTransferSrc)
                    .setSize(size);

            auto buffer = deviceContext->getLogicalDevice().createBuffer(bufferCreateInfo);
            auto memory = makeDeviceMemory(deviceContext, buffer, false);

            void *bufferData;
            auto result = deviceContext->getLogicalDevice().mapMemory(memory, 0, size, vk::MemoryMapFlags(),
                                                                      &bufferData);

            if (result != vk::Result::eSuccess) {
                throw EngineError("Failed to map buffer's memory");
            }

            std::memcpy(bufferData, data, size);
            deviceContext->getLogicalDevice().unmapMemory(memory);

            return std::make_tuple(buffer, memory);
        };

        auto makeTarget = [deviceContext](vk::DeviceSize size, vk::BufferUsageFlags additionalUsage) {
            auto bufferCreateInfo = vk::BufferCreateInfo()
                    .setUsage(vk::BufferUsageFlagBits::eTransferDst | additionalUsage)
                    .setSize(size);

            auto buffer = deviceContext->getLogicalDevice().createBuffer(bufferCreateInfo);
            auto memory = makeDeviceMemory(deviceContext, buffer);

            return std::make_tuple(buffer, memory);
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
                .setCommandPool(deviceContext->getCommandPool());
        auto commandBuffers = deviceContext->getLogicalDevice().allocateCommandBuffers(commandBufferAllocateInfo);

        auto beginInfo = vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
        commandBuffers.at(0).begin(beginInfo);
        commandBuffers.at(0).copyBuffer(stagingVertexBuffer, targetVertexBuffer,
                                        vk::BufferCopy(0, 0, vertexBufferSize));
        commandBuffers.at(0).copyBuffer(stagingIndexBuffer, targetIndexBuffer,
                                        vk::BufferCopy(0, 0, indexBufferSize));
        commandBuffers.at(0).end();

        deviceContext->getGraphicsQueue().submit(vk::SubmitInfo().setCommandBuffers(commandBuffers));
        deviceContext->getGraphicsQueue().waitIdle();

        deviceContext->getLogicalDevice().free(deviceContext->getCommandPool(), commandBuffers);
        deviceContext->getLogicalDevice().free(stagingVertexBufferMemory);
        deviceContext->getLogicalDevice().free(stagingIndexBufferMemory);
        deviceContext->getLogicalDevice().destroy(stagingVertexBuffer);
        deviceContext->getLogicalDevice().destroy(stagingIndexBuffer);

        return new VkMeshAsset(deviceContext,
                               targetVertexBuffer, targetVertexBufferMemory,
                               targetIndexBuffer, targetIndexBufferMemory, indices.size());
    }
}
