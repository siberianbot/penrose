#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_LOGICAL_DEVICE_PROVIDER_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_LOGICAL_DEVICE_PROVIDER_HPP

#include "src/Builtin/Vulkan/Rendering/Objects/VkLogicalDevice.hpp"

namespace Penrose {

    class VkLogicalDeviceProvider {
    public:
        virtual ~VkLogicalDeviceProvider() = default;

        [[nodiscard]] virtual const VkLogicalDevice &getLogicalDevice() const = 0;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_LOGICAL_DEVICE_PROVIDER_HPP
