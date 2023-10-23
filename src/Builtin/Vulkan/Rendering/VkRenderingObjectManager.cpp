#include "VkRenderingObjectManager.hpp"

#include <vulkan/vulkan.hpp>

#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/VkBuffer.hpp"
#include "src/Builtin/Vulkan/Rendering/VkImage.hpp"

namespace Penrose {

    VkRenderingObjectManager::VkRenderingObjectManager(ResourceSet *resources)
            : _bufferFactory(resources->get<VkBufferFactory>()),
              _imageFactory(resources->get<VkImageFactory>()),
              _shaderFactory(resources->get<VkShaderFactory>()),
              _commandManager(resources->get<VkCommandManager>()) {
        //
    }

    Buffer *VkRenderingObjectManager::makeBuffer(BufferType type, std::uint64_t size, std::uint32_t count, void *data) {

        auto staging = dynamic_cast<VkBuffer *>(this->_bufferFactory->makeBuffer(BufferType::Staging,
                                                                                 size, count, true));
        std::memcpy(staging->getPointer(), data, size);

        auto target = dynamic_cast<VkBuffer *>(this->_bufferFactory->makeBuffer(type, size, count, false));

        this->_commandManager->executeTransferOnce(
                [&staging, &target, size](vk::CommandBuffer &commandBuffer) {
                    commandBuffer.copyBuffer(staging->getBuffer(),
                                             target->getBuffer(),
                                             vk::BufferCopy(0, 0, size));

                });

        delete staging;

        return target;
    }

    Image *VkRenderingObjectManager::makeImage(ImageFormat format, std::uint32_t width, std::uint32_t height,
                                               std::vector<std::byte> &&rawData) {

        auto size = width * height * static_cast<std::uint8_t>(format);

        auto buffer = dynamic_cast<VkBuffer *>(this->_bufferFactory->makeBuffer(BufferType::Staging, size, 0, true));
        auto image = dynamic_cast<VkImage *>(this->_imageFactory->makeImage(format, width, height));

        std::memcpy(buffer->getPointer(), rawData.data(), size);

        this->_commandManager->executeTransferOnce(
                [&image, &buffer, width, height](vk::CommandBuffer &commandBuffer) {
                    vk::ImageMemoryBarrier memoryBarrier = vk::ImageMemoryBarrier()
                            .setImage(image->getImage())
                            .setSubresourceRange(vk::ImageSubresourceRange()
                                                         .setAspectMask(vk::ImageAspectFlagBits::eColor)
                                                         .setLayerCount(1)
                                                         .setBaseArrayLayer(0)
                                                         .setLevelCount(1)
                                                         .setBaseMipLevel(0));

                    memoryBarrier = memoryBarrier
                            .setSrcAccessMask(vk::AccessFlagBits::eNone)
                            .setDstAccessMask(vk::AccessFlagBits::eTransferWrite)
                            .setOldLayout(vk::ImageLayout::eUndefined)
                            .setNewLayout(vk::ImageLayout::eTransferDstOptimal);
                    commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
                                                  vk::PipelineStageFlagBits::eTransfer,
                                                  vk::DependencyFlagBits(0),
                                                  {}, {}, memoryBarrier);

                    auto region = vk::BufferImageCopy()
                            .setBufferOffset(0)
                            .setBufferRowLength(width)
                            .setBufferImageHeight(height)
                            .setImageExtent(vk::Extent3D(width, height, 1))
                            .setImageOffset(vk::Offset3D(0, 0, 0))
                            .setImageSubresource(vk::ImageSubresourceLayers()
                                                         .setLayerCount(1)
                                                         .setBaseArrayLayer(0)
                                                         .setMipLevel(0)
                                                         .setAspectMask(vk::ImageAspectFlagBits::eColor));
                    commandBuffer.copyBufferToImage(buffer->getBuffer(), image->getImage(),
                                                    vk::ImageLayout::eTransferDstOptimal, region);

                    memoryBarrier = memoryBarrier
                            .setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
                            .setDstAccessMask(vk::AccessFlagBits::eShaderRead)
                            .setOldLayout(vk::ImageLayout::eTransferDstOptimal)
                            .setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal);
                    commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer,
                                                  vk::PipelineStageFlagBits::eFragmentShader,
                                                  vk::DependencyFlagBits(0),
                                                  {}, {}, memoryBarrier);
                });

        delete buffer;

        return image;
    }

    Shader *VkRenderingObjectManager::makeShader(std::vector<std::byte> &&rawData) {
        return this->_shaderFactory->makeShader(reinterpret_cast<const std::uint32_t *>(rawData.data()),
                                                rawData.size());
    }
}
