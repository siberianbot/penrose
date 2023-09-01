#include "VkImageAssetFactory.hpp"

#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Rendering/Utils.hpp"

#include "src/Builtin/Vulkan/Rendering/VkBuffer.hpp"
#include "src/Builtin/Vulkan/Rendering/VkImage.hpp"

namespace Penrose {

    VkImageAssetFactory::VkImageAssetFactory(ResourceSet *resources)
            : _bufferFactory(resources->getLazy<BufferFactory>()),
              _deviceContext(resources->getLazy<DeviceContext>()),
              _imageFactory(resources->getLazy<ImageFactory>()) {
        //
    }

    ImageAsset *VkImageAssetFactory::makeImage(ImageFormat format, std::uint32_t width, std::uint32_t height,
                                               void *data) {
        auto size = width * height * getChannels(format);

        auto buffer = dynamic_cast<VkBuffer *>(this->_bufferFactory->makeBuffer(BufferType::Staging, size, 0, true));
        auto image = dynamic_cast<VkImage *>(this->_imageFactory->makeImage(format, width, height));

        std::memcpy(buffer->getPointer(), data, size);

        auto range = vk::ImageSubresourceRange()
                .setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setLayerCount(1)
                .setBaseArrayLayer(0)
                .setLevelCount(1)
                .setBaseMipLevel(0);

        auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo()
                .setCommandBufferCount(1)
                .setCommandPool(this->_deviceContext->getCommandPool());
        auto commandBuffers = this->_deviceContext->getLogicalDevice()
                .allocateCommandBuffers(commandBufferAllocateInfo);

        auto beginInfo = vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
        commandBuffers.at(0).begin(beginInfo);

        vk::ImageMemoryBarrier memoryBarrier;

        memoryBarrier = vk::ImageMemoryBarrier()
                .setImage(image->getImage())
                .setSrcAccessMask(vk::AccessFlagBits::eNone)
                .setDstAccessMask(vk::AccessFlagBits::eTransferWrite)
                .setOldLayout(vk::ImageLayout::eUndefined)
                .setNewLayout(vk::ImageLayout::eTransferDstOptimal)
                .setSubresourceRange(range);
        commandBuffers.at(0).pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
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
        commandBuffers.at(0).copyBufferToImage(buffer->getBuffer(), image->getImage(),
                                               vk::ImageLayout::eTransferDstOptimal, region);

        memoryBarrier = vk::ImageMemoryBarrier()
                .setImage(image->getImage())
                .setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
                .setDstAccessMask(vk::AccessFlagBits::eShaderRead)
                .setOldLayout(vk::ImageLayout::eTransferDstOptimal)
                .setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                .setSubresourceRange(range);
        commandBuffers.at(0).pipelineBarrier(vk::PipelineStageFlagBits::eTransfer,
                                             vk::PipelineStageFlagBits::eFragmentShader,
                                             vk::DependencyFlagBits(0),
                                             {}, {}, memoryBarrier);

        commandBuffers.at(0).end();

        this->_deviceContext->getGraphicsQueue().submit(vk::SubmitInfo().setCommandBuffers(commandBuffers));
        this->_deviceContext->getGraphicsQueue().waitIdle();

        this->_deviceContext->getLogicalDevice().free(this->_deviceContext->getCommandPool(), commandBuffers);

        delete buffer;

        return new ImageAsset(std::unique_ptr<Image>(image));
    }
}
