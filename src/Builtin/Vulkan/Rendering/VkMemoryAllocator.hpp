#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_MEMORY_ALLOCATOR_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_MEMORY_ALLOCATOR_HPP

#include <vulkan/vulkan.hpp>

#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkPhysicalDeviceContext.hpp"

namespace Penrose {

    class VkMemoryAllocator : public Resource<VkMemoryAllocator> {
    public:
        explicit VkMemoryAllocator(ResourceSet *resources);
        ~VkMemoryAllocator() override = default;

        [[nodiscard]] vk::DeviceMemory allocate(const vk::Buffer &buffer, bool local = true);
        [[nodiscard]] vk::DeviceMemory allocate(const vk::Image &image, bool local = true);

    private:
        ResourceProxy<VkLogicalDeviceContext> _logicalDeviceContext;
        ResourceProxy<VkPhysicalDeviceContext> _physicalDeviceContext;

        [[nodiscard]] vk::DeviceMemory allocate(const vk::MemoryRequirements &requirements, bool local);
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_MEMORY_ALLOCATOR_HPP
