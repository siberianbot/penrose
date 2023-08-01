#include "VkImageAsset.hpp"

#include <Penrose/Common/EngineError.hpp>

#include "src/Rendering/DeviceContext.hpp"

#include <src/Builtin/Rendering/VkUtils.hpp>

namespace Penrose {

    VkImageAsset::VkImageAsset(DeviceContext *deviceContext,
                               vk::Image image, vk::DeviceMemory imageMemory, vk::ImageView imageView)
            : _deviceContext(deviceContext),
              _image(image), _imageMemory(imageMemory), _imageView(imageView) {
        //
    }

    VkImageAsset::~VkImageAsset() {
        this->_deviceContext->getLogicalDevice().destroy(this->_imageView);
        this->_deviceContext->getLogicalDevice().free(this->_imageMemory);
        this->_deviceContext->getLogicalDevice().destroy(this->_image);
    }

    VkImageAsset *makeVkImageAsset(DeviceContext *deviceContext,
                                   std::uint32_t width, std::uint32_t height, std::uint32_t channels, void *data) {
        auto bufferSize = width * height * channels;

        auto bufferCreateInfo = vk::BufferCreateInfo()
                .setUsage(vk::BufferUsageFlagBits::eTransferSrc)
                .setSize(bufferSize);
        auto buffer = deviceContext->getLogicalDevice().createBuffer(bufferCreateInfo);
        auto bufferMemory = makeDeviceMemory(deviceContext, buffer, false);

        void *bufferData;
        auto result = deviceContext->getLogicalDevice().mapMemory(bufferMemory, 0, bufferSize,
                                                                  vk::MemoryMapFlags(), &bufferData);

        if (result != vk::Result::eSuccess) {
            throw EngineError("Failed to map buffer's memory");
        }

        std::memcpy(bufferData, data, bufferSize);
        deviceContext->getLogicalDevice().unmapMemory(bufferMemory);

        auto imageCreateInfo = vk::ImageCreateInfo()
                .setUsage(vk::ImageUsageFlagBits::eTransferSrc |
                          vk::ImageUsageFlagBits::eTransferDst |
                          vk::ImageUsageFlagBits::eSampled)
                .setImageType(vk::ImageType::e2D)
                .setExtent(vk::Extent3D(width, height, 1))
                .setFormat(vk::Format::eR8G8B8A8Srgb)
                .setSamples(vk::SampleCountFlagBits::e1)
                .setArrayLayers(1)
                .setMipLevels(1)
                .setSharingMode(vk::SharingMode::eExclusive)
                .setTiling(vk::ImageTiling::eOptimal);
        auto image = deviceContext->getLogicalDevice().createImage(imageCreateInfo);
        auto imageMemory = makeDeviceMemory(deviceContext, image);

        auto range = vk::ImageSubresourceRange()
                .setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setLayerCount(1)
                .setBaseArrayLayer(0)
                .setLevelCount(1)
                .setBaseMipLevel(0);

        auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo()
                .setCommandBufferCount(1)
                .setCommandPool(deviceContext->getCommandPool());
        auto commandBuffers = deviceContext->getLogicalDevice().allocateCommandBuffers(commandBufferAllocateInfo);

        auto beginInfo = vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
        commandBuffers.at(0).begin(beginInfo);

        vk::ImageMemoryBarrier memoryBarrier;

        memoryBarrier = vk::ImageMemoryBarrier()
                .setImage(image)
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
        commandBuffers.at(0).copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, region);

        memoryBarrier = vk::ImageMemoryBarrier()
                .setImage(image)
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

        deviceContext->getGraphicsQueue().submit(vk::SubmitInfo().setCommandBuffers(commandBuffers));
        deviceContext->getGraphicsQueue().waitIdle();

        deviceContext->getLogicalDevice().free(deviceContext->getCommandPool(), commandBuffers);
        deviceContext->getLogicalDevice().free(bufferMemory);
        deviceContext->getLogicalDevice().destroy(buffer);

        auto imageViewCreateInfo = vk::ImageViewCreateInfo()
                .setViewType(vk::ImageViewType::e2D)
                .setImage(image)
                .setFormat(vk::Format::eR8G8B8A8Srgb)
                .setSubresourceRange(range)
                .setComponents(vk::ComponentMapping());
        auto imageView = deviceContext->getLogicalDevice().createImageView(imageViewCreateInfo);

        return new VkImageAsset(deviceContext,
                                image, imageMemory, imageView);
    }
}
