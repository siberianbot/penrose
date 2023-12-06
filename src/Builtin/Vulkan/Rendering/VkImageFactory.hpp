#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_IMAGE_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_IMAGE_FACTORY_HPP

#include <Penrose/Rendering/ImageFactory.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkMemoryAllocator.hpp"

namespace Penrose {

    class VkImageFactory : public Resource<VkImageFactory>,
                           public ImageFactory {
    public:
        explicit VkImageFactory(const ResourceSet *resources);
        ~VkImageFactory() override = default;

        [[nodiscard]] Image *makeImage(ImageFormat format, std::uint32_t width, std::uint32_t height) override;

    private:
        ResourceProxy<VkLogicalDeviceContext> _logicalDeviceContext;
        ResourceProxy<VkMemoryAllocator> _memoryAllocator;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_IMAGE_FACTORY_HPP
