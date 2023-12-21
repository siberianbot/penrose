#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_PHYSICAL_DEVICE_SELECTOR_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_PHYSICAL_DEVICE_SELECTOR_HPP

#include <optional>

#include <vulkan/vulkan.hpp>

#include <Penrose/Common/Log.hpp>
#include <Penrose/Rendering/SurfaceManager.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include <Penrose/Builtin/Vulkan/VkSurfaceProvider.hpp>

#include "src/Builtin/Vulkan/Rendering/Objects/VkPhysicalDevice.hpp"
#include "src/Builtin/Vulkan/VulkanBackend.hpp"

namespace Penrose {

    class VkPhysicalDeviceSelector final: public Resource<VkPhysicalDeviceSelector> {
    public:
        explicit VkPhysicalDeviceSelector(const ResourceSet *resources);
        ~VkPhysicalDeviceSelector() override = default;

        [[nodiscard]] VkPhysicalDevice selectPhysicalDevice();

    private:
        ResourceProxy<Log> _log;
        ResourceProxy<SurfaceManager> _surfaceManager;
        ResourceProxy<VkSurfaceProvider> _vkSurfaceProvider;
        ResourceProxy<VulkanBackend> _vulkanBackend;

        [[nodiscard]] std::optional<VkPhysicalDevice> makePhysicalDevice(
            const vk::PhysicalDevice &device, const vk::SurfaceKHR &surface
        );
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_PHYSICAL_DEVICE_SELECTOR_HPP
