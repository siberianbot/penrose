#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_MEMORY_ALLOCATOR_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_MEMORY_ALLOCATOR_HPP

#include <vulkan/vulkan.hpp>

#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkPhysicalDeviceContext.hpp"

namespace Penrose {

    class ResourceSet;

    class VkMemoryAllocator : public Resource {
    public:
        explicit VkMemoryAllocator(ResourceSet *resources);
        ~VkMemoryAllocator() override = default;

        [[nodiscard]] vk::DeviceMemory allocate(const vk::Buffer &buffer, bool local = true);
        [[nodiscard]] vk::DeviceMemory allocate(const vk::Image &image, bool local = true);

    private:
        Lazy<VkLogicalDeviceContext> _logicalDeviceContext;
        Lazy<VkPhysicalDeviceContext> _physicalDeviceContext;

        [[nodiscard]] vk::DeviceMemory allocate(const vk::MemoryRequirements &requirements, bool local);
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_MEMORY_ALLOCATOR_HPP
