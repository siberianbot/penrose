#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_LOGICAL_DEVICE_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_LOGICAL_DEVICE_FACTORY_HPP

#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/Rendering/Objects/VkLogicalDevice.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkPhysicalDevice.hpp"

namespace Penrose {

    class VkLogicalDeviceFactory final: public Resource<VkLogicalDeviceFactory> {
    public:
        ~VkLogicalDeviceFactory() override = default;

        [[nodiscard]] VkLogicalDevice makeLogicalDevice(const VkPhysicalDevice &physicalDevice);
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_LOGICAL_DEVICE_FACTORY_HPP
