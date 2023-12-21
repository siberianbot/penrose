#include "VkImageFactory.hpp"

#include "src/Builtin/Vulkan/Rendering/Objects/VkImage.hpp"
#include "src/Builtin/Vulkan/Rendering/VkUtils.hpp"

namespace Penrose {

    VkImageFactory::VkImageFactory(const ResourceSet *resources)
        : _physicalDeviceProvider(resources->get<VkPhysicalDeviceProvider>()),
          _logicalDeviceProvider(resources->get<VkLogicalDeviceProvider>()),
          _memoryAllocator(resources->get<VkMemoryAllocator>()),
          _bufferFactory(resources->get<VkBufferFactory>()) {
        //
    }

    void VkImageFactory::init() {
        const auto createInfo = vk::CommandPoolCreateInfo().setQueueFamilyIndex(
            this->_physicalDeviceProvider->getPhysicalDevice().transferFamilyIdx
        );

        this->_commandPool = this->_logicalDeviceProvider->getLogicalDevice().handle->createCommandPoolUnique(createInfo
        );
    }

    void VkImageFactory::destroy() {
        this->_commandPool.reset();
    }

    Image *VkImageFactory::makeImage(const ImageFormat format, const std::uint32_t width, const std::uint32_t height) {
        const auto vkFormat = mapImageFormat(format);

        const auto imageCreateInfo = vk::ImageCreateInfo()
                                         .setUsage(
                                             vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst
                                             | vk::ImageUsageFlagBits::eSampled
                                         )
                                         .setImageType(vk::ImageType::e2D)
                                         .setExtent(vk::Extent3D(width, height, 1))
                                         .setFormat(vkFormat)
                                         .setSamples(vk::SampleCountFlagBits::e1)
                                         .setArrayLayers(1)
                                         .setMipLevels(1)
                                         .setSharingMode(vk::SharingMode::eExclusive)
                                         .setTiling(vk::ImageTiling::eOptimal);

        const auto imageViewCreateInfo = vk::ImageViewCreateInfo()
                                             .setViewType(vk::ImageViewType::e2D)
                                             .setFormat(vkFormat)
                                             .setSubresourceRange(vk::ImageSubresourceRange()
                                                                      .setAspectMask(vk::ImageAspectFlagBits::eColor)
                                                                      .setLayerCount(1)
                                                                      .setBaseArrayLayer(0)
                                                                      .setLevelCount(1)
                                                                      .setBaseMipLevel(0))
                                             .setComponents(vk::ComponentMapping());

        auto [image, imageMemory, imageView] = this->makeImage(imageCreateInfo, imageViewCreateInfo);

        return new VkImage(format, width, height, std::move(image), std::move(imageMemory), std::move(imageView));
    }

    Image *VkImageFactory::makeImage(
        const ImageFormat format, const std::uint32_t width, const std::uint32_t height,
        std::vector<std::byte> &&rawData
    ) {
        const auto vkFormat = mapImageFormat(format);

        const auto imageCreateInfo = vk::ImageCreateInfo()
                                         .setUsage(
                                             vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst
                                             | vk::ImageUsageFlagBits::eSampled
                                         )
                                         .setImageType(vk::ImageType::e2D)
                                         .setExtent(vk::Extent3D(width, height, 1))
                                         .setFormat(vkFormat)
                                         .setSamples(vk::SampleCountFlagBits::e1)
                                         .setArrayLayers(1)
                                         .setMipLevels(1)
                                         .setSharingMode(vk::SharingMode::eExclusive)
                                         .setTiling(vk::ImageTiling::eOptimal);

        constexpr auto range = vk::ImageSubresourceRange()
                                   .setAspectMask(vk::ImageAspectFlagBits::eColor)
                                   .setLayerCount(1)
                                   .setBaseArrayLayer(0)
                                   .setLevelCount(1)
                                   .setBaseMipLevel(0);

        const auto imageViewCreateInfo = vk::ImageViewCreateInfo()
                                             .setViewType(vk::ImageViewType::e2D)
                                             .setFormat(vkFormat)
                                             .setSubresourceRange(range)
                                             .setComponents(vk::ComponentMapping());

        const auto size = width * height * static_cast<std::uint32_t>(format);

        auto [buffer, bufferMemory, data] = this->_bufferFactory->makeBuffer(
            vk::BufferUsageFlagBits::eTransferSrc, size, true
        );
        auto [image, imageMemory, imageView] = this->makeImage(imageCreateInfo, imageViewCreateInfo);

        std::memcpy(data, rawData.data(), size);

        {
            auto &device = this->_logicalDeviceProvider->getLogicalDevice().handle;
            auto &transferQueue = this->_logicalDeviceProvider->getLogicalDevice().transferQueue;

            const auto allocateInfo = vk::CommandBufferAllocateInfo()
                                          .setLevel(vk::CommandBufferLevel::ePrimary)
                                          .setCommandPool(this->_commandPool.get())
                                          .setCommandBufferCount(1);

            auto commandBuffer = std::move(device->allocateCommandBuffersUnique(allocateInfo).at(0));

            const auto topOfPipeMemoryBarrier = vk::ImageMemoryBarrier()
                                                    .setImage(image.get())
                                                    .setSrcAccessMask(vk::AccessFlagBits::eNone)
                                                    .setDstAccessMask(vk::AccessFlagBits::eTransferWrite)
                                                    .setOldLayout(vk::ImageLayout::eUndefined)
                                                    .setNewLayout(vk::ImageLayout::eTransferDstOptimal)
                                                    .setSubresourceRange(range);

            const auto transferMemoryBarrier = vk::ImageMemoryBarrier()
                                                   .setImage(image.get())
                                                   .setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
                                                   .setDstAccessMask(vk::AccessFlagBits::eShaderRead)
                                                   .setOldLayout(vk::ImageLayout::eTransferDstOptimal)
                                                   .setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                                                   .setSubresourceRange(range);

            const auto region = vk::BufferImageCopy()
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

            commandBuffer->begin(vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));

            commandBuffer->pipelineBarrier(
                vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer, {}, {}, {},
                topOfPipeMemoryBarrier
            );

            commandBuffer->copyBufferToImage(buffer.get(), image.get(), vk::ImageLayout::eTransferDstOptimal, region);

            commandBuffer->pipelineBarrier(
                vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, {}, {}, {},
                transferMemoryBarrier
            );

            commandBuffer->end();

            transferQueue.submit(vk::SubmitInfo().setCommandBuffers(commandBuffer.get()));
            transferQueue.waitIdle();
        }

        return new VkImage(format, width, height, std::move(image), std::move(imageMemory), std::move(imageView));
    }

    VkImageInternal VkImageFactory::makeImage(const TargetInfo &target, const VkSwapchain &swapchain) {
        const auto format = mapRenderFormat(target.format, swapchain.format);
        const auto extent = vk::Extent3D(mapSize(target.size, swapchain.extent));

        const auto imageCreateInfo = vk::ImageCreateInfo()
                                         .setUsage(mapTargetUsage(target.usage))
                                         .setImageType(vk::ImageType::e2D)
                                         .setExtent(extent)
                                         .setFormat(format)
                                         .setSamples(vk::SampleCountFlagBits::e1)
                                         .setArrayLayers(1)
                                         .setMipLevels(1)
                                         .setSharingMode(vk::SharingMode::eExclusive)
                                         .setTiling(vk::ImageTiling::eOptimal);

        const auto imageViewCreateInfo = vk::ImageViewCreateInfo()
                                             .setViewType(vk::ImageViewType::e2D)
                                             .setFormat(format)
                                             .setSubresourceRange(vk::ImageSubresourceRange()
                                                                      .setAspectMask(mapTargetUsageAspect(target.usage))
                                                                      .setLayerCount(1)
                                                                      .setBaseArrayLayer(0)
                                                                      .setLevelCount(1)
                                                                      .setBaseMipLevel(0))
                                             .setComponents(vk::ComponentMapping());

        return this->makeImage(imageCreateInfo, imageViewCreateInfo);
    }

    VkImageInternal VkImageFactory::makeImage(
        const vk::ImageCreateInfo &imageCreateInfo, vk::ImageViewCreateInfo imageViewCreateInfo
    ) {
        auto &device = this->_logicalDeviceProvider->getLogicalDevice().handle;

        auto image = device->createImageUnique(imageCreateInfo);
        auto imageMemory = this->_memoryAllocator->allocateImage(image.get());
        auto imageView = device->createImageViewUnique(imageViewCreateInfo.setImage(image.get()));

        return {
            std::move(image),
            std::move(imageMemory),
            std::move(imageView),
        };
    }
}
