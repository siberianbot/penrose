#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_PHYSICAL_DEVICE_PROVIDER_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_PHYSICAL_DEVICE_PROVIDER_HPP

#include "src/Builtin/Vulkan/Rendering/Objects/VkPhysicalDevice.hpp"

namespace Penrose {

    class VkPhysicalDeviceProvider {
    public:
        virtual ~VkPhysicalDeviceProvider() = default;

        [[nodiscard]] virtual const VkPhysicalDevice &getPhysicalDevice() const = 0;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_PHYSICAL_DEVICE_PROVIDER_HPP
