#include "VkMeshAssetFactory.hpp"

#include <vulkan/vulkan.hpp>

#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/VkBuffer.hpp"

namespace Penrose {

    VkMeshAssetFactory::VkMeshAssetFactory(ResourceSet *resources)
            : _bufferFactory(resources->getLazy<BufferFactory>()),
              _deviceContext(resources->getLazy<DeviceContext>()),
              _logicalDeviceContext(resources->getLazy<VkLogicalDeviceContext>()) {
        //
    }

    MeshAsset *VkMeshAssetFactory::makeMesh(std::vector<Vertex> &&vertices,
                                            std::vector<std::uint32_t> &&indices) {
        auto vertexBufferSize = sizeof(Vertex) * vertices.size();
        auto indexBufferSize = sizeof(std::uint32_t) * indices.size();

        auto stagingVertexBuffer = dynamic_cast<VkBuffer *>(this->_bufferFactory->makeBuffer(BufferType::Staging,
                                                                                             vertexBufferSize,
                                                                                             vertices.size(),
                                                                                             true));
        std::memcpy(stagingVertexBuffer->getPointer(), vertices.data(), vertexBufferSize);

        auto stagingIndexBuffer = dynamic_cast<VkBuffer *>(this->_bufferFactory->makeBuffer(BufferType::Staging,
                                                                                            indexBufferSize,
                                                                                            indices.size(),
                                                                                            true));
        std::memcpy(stagingIndexBuffer->getPointer(), indices.data(), indexBufferSize);

        auto targetVertexBuffer = dynamic_cast<VkBuffer *>(this->_bufferFactory->makeBuffer(BufferType::Vertex,
                                                                                            vertexBufferSize,
                                                                                            vertices.size(),
                                                                                            false));
        auto targetIndexBuffer = dynamic_cast<VkBuffer *>(this->_bufferFactory->makeBuffer(BufferType::Index,
                                                                                           indexBufferSize,
                                                                                           indices.size(),
                                                                                           false));
        auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo()
                .setCommandBufferCount(1)
                .setCommandPool(this->_deviceContext->getCommandPool());
        auto commandBuffers = this->_logicalDeviceContext->getHandle()
                .allocateCommandBuffers(commandBufferAllocateInfo);

        auto beginInfo = vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
        commandBuffers.at(0).begin(beginInfo);
        commandBuffers.at(0).copyBuffer(stagingVertexBuffer->getBuffer(), targetVertexBuffer->getBuffer(),
                                        vk::BufferCopy(0, 0, vertexBufferSize));
        commandBuffers.at(0).copyBuffer(stagingIndexBuffer->getBuffer(), targetIndexBuffer->getBuffer(),
                                        vk::BufferCopy(0, 0, indexBufferSize));
        commandBuffers.at(0).end();

        this->_logicalDeviceContext->getGraphicsQueue().submit(vk::SubmitInfo().setCommandBuffers(commandBuffers));
        this->_logicalDeviceContext->getGraphicsQueue().waitIdle();

        this->_logicalDeviceContext->getHandle().free(this->_deviceContext->getCommandPool(), commandBuffers);

        delete stagingVertexBuffer;
        delete stagingIndexBuffer;

        return new MeshAsset(std::unique_ptr<Buffer>(targetVertexBuffer),
                             std::unique_ptr<Buffer>(targetIndexBuffer));
    }
}
