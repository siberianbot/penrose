#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_IMAGE_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_IMAGE_FACTORY_HPP

#include <Penrose/Rendering/ImageFactory.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkMemoryAllocator.hpp"

namespace Penrose {

    class ResourceSet;

    class VkImageFactory : public Resource, public ImageFactory {
    public:
        explicit VkImageFactory(ResourceSet *resources);
        ~VkImageFactory() override = default;

        [[nodiscard]] Image *makeImage(ImageFormat format, std::uint32_t width, std::uint32_t height) override;

    private:
        Lazy<VkLogicalDeviceContext> _logicalDeviceContext;
        Lazy<VkMemoryAllocator> _memoryAllocator;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_IMAGE_FACTORY_HPP
