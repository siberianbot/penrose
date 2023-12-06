#include "VkImageFactory.hpp"

#include "src/Builtin/Vulkan/Rendering/VkImage.hpp"
#include "src/Builtin/Vulkan/Rendering/VkUtils.hpp"

namespace Penrose {

    VkImageFactory::VkImageFactory(const ResourceSet *resources)
            : _logicalDeviceContext(resources->get<VkLogicalDeviceContext>()),
              _memoryAllocator(resources->get<VkMemoryAllocator>()) {
        //
    }

    Image *VkImageFactory::makeImage(ImageFormat format, std::uint32_t width, std::uint32_t height) {
        auto vkFormat = toVkFormat(format);

        auto imageCreateInfo = vk::ImageCreateInfo()
                .setUsage(vk::ImageUsageFlagBits::eTransferSrc |
                          vk::ImageUsageFlagBits::eTransferDst |
                          vk::ImageUsageFlagBits::eSampled)
                .setImageType(vk::ImageType::e2D)
                .setExtent(vk::Extent3D(width, height, 1))
                .setFormat(vkFormat)
                .setSamples(vk::SampleCountFlagBits::e1)
                .setArrayLayers(1)
                .setMipLevels(1)
                .setSharingMode(vk::SharingMode::eExclusive)
                .setTiling(vk::ImageTiling::eOptimal);

        auto image = this->_logicalDeviceContext->getHandle().createImage(imageCreateInfo);
        auto imageMemory = this->_memoryAllocator->allocate(image);

        auto imageViewCreateInfo = vk::ImageViewCreateInfo()
                .setViewType(vk::ImageViewType::e2D)
                .setImage(image)
                .setFormat(vkFormat)
                .setSubresourceRange(vk::ImageSubresourceRange()
                                             .setAspectMask(vk::ImageAspectFlagBits::eColor)
                                             .setLayerCount(1)
                                             .setBaseArrayLayer(0)
                                             .setLevelCount(1)
                                             .setBaseMipLevel(0))
                .setComponents(vk::ComponentMapping());
        auto imageView = this->_logicalDeviceContext->getHandle().createImageView(imageViewCreateInfo);

        return new VkImage(this->_logicalDeviceContext.get(),
                           format, width, height,
                           image, imageMemory, imageView);
    }
}
