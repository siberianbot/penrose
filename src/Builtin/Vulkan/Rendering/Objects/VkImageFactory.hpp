#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_IMAGE_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_IMAGE_FACTORY_HPP

#include <tuple>

#include <Penrose/Rendering/Graph/TargetInfo.hpp>
#include <Penrose/Rendering/Objects/ImageFactory.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/Objects/VkBufferFactory.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkLogicalDeviceProvider.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkPhysicalDeviceProvider.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkSwapchain.hpp"
#include "src/Builtin/Vulkan/Rendering/VkMemoryAllocator.hpp"

namespace Penrose {

    using VkImageInternal = std::tuple<vk::UniqueImage, vk::UniqueDeviceMemory, vk::UniqueImageView>;

    class VkImageFactory final: public Resource<VkImageFactory>,
                                public ImageFactory {
    public:
        explicit VkImageFactory(const ResourceSet *resources);
        ~VkImageFactory() override = default;

        void init();
        void destroy();

        [[nodiscard]] Image *makeImage(ImageFormat format, std::uint32_t width, std::uint32_t height) override;
        [[nodiscard]] Image *makeImage(
            ImageFormat format, std::uint32_t width, std::uint32_t height, std::vector<std::byte> &&rawData
        ) override;

        [[nodiscard]] VkImageInternal makeImage(const TargetInfo &target, const VkSwapchain &swapchain);

    private:
        ResourceProxy<VkPhysicalDeviceProvider> _physicalDeviceProvider;
        ResourceProxy<VkLogicalDeviceProvider> _logicalDeviceProvider;
        ResourceProxy<VkMemoryAllocator> _memoryAllocator;
        ResourceProxy<VkBufferFactory> _bufferFactory;

        [[nodiscard]] VkImageInternal makeImage(
            const vk::ImageCreateInfo &imageCreateInfo, vk::ImageViewCreateInfo imageViewCreateInfo
        );

        vk::UniqueCommandPool _commandPool;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_IMAGE_FACTORY_HPP
