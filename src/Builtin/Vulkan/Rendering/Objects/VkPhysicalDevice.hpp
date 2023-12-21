#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_PHYSICAL_DEVICE_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_PHYSICAL_DEVICE_HPP

#include <cstdint>
#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace Penrose {

    struct VkPhysicalDevice {
        vk::PhysicalDevice handle;
        vk::PhysicalDeviceFeatures features;
        vk::PhysicalDeviceProperties properties;
        std::uint32_t graphicsFamilyIdx;
        std::uint32_t transferFamilyIdx;
        std::uint32_t presentFamilyIdx;
        std::vector<std::string> extensions;
        std::uint32_t priority;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_PHYSICAL_DEVICE_HPP
