#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_SWAPCHAIN_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_SWAPCHAIN_FACTORY_HPP

#include <optional>

#include <Penrose/Rendering/SurfaceManager.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/VkSurfaceProvider.hpp"
#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchain.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainPreferencesProvider.hpp"

namespace Penrose {

    class VkSwapchainFactory : public Resource<VkSwapchainFactory> {
    public:
        explicit VkSwapchainFactory(const ResourceSet *resources);
        ~VkSwapchainFactory() override = default;

        [[nodiscard]] VkSwapchain *makeSwapchain(std::optional<VkSwapchain *> oldSwapchain = std::nullopt);

    private:
        ResourceProxy<VkLogicalDeviceContext> _logicalDeviceContext;
        ResourceProxy<SurfaceManager> _surfaceManager;
        ResourceProxy<VkSurfaceProvider> _surfaceProvider;
        ResourceProxy<VkSwapchainPreferencesProvider> _swapchainPreferencesProvider;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_SWAPCHAIN_FACTORY_HPP
