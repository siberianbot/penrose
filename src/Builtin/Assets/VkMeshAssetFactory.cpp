#include "VkMeshAssetFactory.hpp"

#include <memory>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/BufferFactory.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Rendering/DeviceContext.hpp"

#include "src/Builtin/Rendering/VkBuffer.hpp"

namespace Penrose {

    VkMeshAssetFactory::VkMeshAssetFactory(ResourceSet *resources)
            : _bufferFactory(resources->get<BufferFactory>()),
              _deviceContext(resources->get<DeviceContext>()) {
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
        auto commandBuffers = this->_deviceContext->getLogicalDevice()
                .allocateCommandBuffers(commandBufferAllocateInfo);

        auto beginInfo = vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
        commandBuffers.at(0).begin(beginInfo);
        commandBuffers.at(0).copyBuffer(stagingVertexBuffer->getBuffer(), targetVertexBuffer->getBuffer(),
                                        vk::BufferCopy(0, 0, vertexBufferSize));
        commandBuffers.at(0).copyBuffer(stagingIndexBuffer->getBuffer(), targetIndexBuffer->getBuffer(),
                                        vk::BufferCopy(0, 0, indexBufferSize));
        commandBuffers.at(0).end();

        this->_deviceContext->getGraphicsQueue().submit(vk::SubmitInfo().setCommandBuffers(commandBuffers));
        this->_deviceContext->getGraphicsQueue().waitIdle();

        this->_deviceContext->getLogicalDevice().free(this->_deviceContext->getCommandPool(), commandBuffers);

        delete stagingVertexBuffer;
        delete stagingIndexBuffer;

        return new MeshAsset(std::unique_ptr<Buffer>(targetVertexBuffer),
                             std::unique_ptr<Buffer>(targetIndexBuffer));
    }
}
