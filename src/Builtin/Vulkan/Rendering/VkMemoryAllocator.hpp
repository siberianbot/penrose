#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_MEMORY_ALLOCATOR_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_MEMORY_ALLOCATOR_HPP

#include <vulkan/vulkan.hpp>

#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/Objects/VkLogicalDeviceProvider.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkPhysicalDeviceProvider.hpp"

namespace Penrose {

    class VkMemoryAllocator final: public Resource<VkMemoryAllocator> {
    public:
        explicit VkMemoryAllocator(const ResourceSet *resources);
        ~VkMemoryAllocator() override = default;

        [[nodiscard]] vk::UniqueDeviceMemory allocateBuffer(const vk::Buffer &buffer, bool local);
        [[nodiscard]] vk::UniqueDeviceMemory allocateImage(const vk::Image &image);

    private:
        ResourceProxy<VkLogicalDeviceProvider> _logicalDeviceProvider;
        ResourceProxy<VkPhysicalDeviceProvider> _physicalDeviceProvider;

        [[nodiscard]] vk::UniqueDeviceMemory allocate(const vk::MemoryRequirements &requirements, bool local);
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_MEMORY_ALLOCATOR_HPP
