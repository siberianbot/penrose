#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_SWAPCHAIN_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_SWAPCHAIN_FACTORY_HPP

#include <memory>

#include <Penrose/Rendering/Surface.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include <Penrose/Builtin/Vulkan/VkSurfaceProvider.hpp>

#include "src/Builtin/Vulkan/Rendering/Objects/VkLogicalDeviceProvider.hpp"
#include "src/Builtin/Vulkan/Rendering/Objects/VkSwapchain.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSurfacePreferencesProvider.hpp"

namespace Penrose {

    class VkSwapchainFactory final: public Resource<VkSwapchainFactory> {
    public:
        explicit VkSwapchainFactory(const ResourceSet *resources);
        ~VkSwapchainFactory() override = default;

        [[nodiscard]] VkSwapchain makeSwapchain(const std::shared_ptr<Surface> &surface);

        [[nodiscard]] VkSwapchain makeSwapchain(const VkSwapchain &oldSwapchain);

    private:
        ResourceProxy<VkLogicalDeviceProvider> _logicalDeviceProvider;
        ResourceProxy<VkSurfaceProvider> _surfaceProvider;
        ResourceProxy<VkSurfacePreferencesProvider> _surfacePreferencesProvider;

        [[nodiscard]] VkSwapchain makeSwapchain(
            const std::shared_ptr<Surface> &surface, const vk::SwapchainCreateInfoKHR &swapchainCreateInfo
        );
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_SWAPCHAIN_FACTORY_HPP
