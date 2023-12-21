#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_LOGICAL_DEVICE_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_LOGICAL_DEVICE_HPP

#include <vulkan/vulkan.hpp>

namespace Penrose {

    struct VkLogicalDevice {
        vk::UniqueDevice handle;
        vk::Queue graphicsQueue;
        vk::Queue transferQueue;
        vk::Queue presentQueue;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_LOGICAL_DEVICE_HPP
