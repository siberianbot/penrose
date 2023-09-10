#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_SWAPCHAIN_FACTORY_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_SWAPCHAIN_FACTORY_HPP

#include <optional>

#include <Penrose/Rendering/SurfaceManager.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/VkSurfaceProvider.hpp"
#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchain.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainPreferencesProvider.hpp"

namespace Penrose {

    class ResourceSet;

    class VkSwapchainFactory : public Resource {
    public:
        explicit VkSwapchainFactory(ResourceSet *resources);
        ~VkSwapchainFactory() override = default;

        [[nodiscard]] VkSwapchain *makeSwapchain(std::optional<VkSwapchain *> oldSwapchain = std::nullopt);

    private:
        Lazy<VkLogicalDeviceContext> _logicalDeviceContext;
        Lazy<SurfaceManager> _surfaceManager;
        Lazy<VkSurfaceProvider> _surfaceProvider;
        Lazy<VkSwapchainPreferencesProvider> _swapchainPreferencesProvider;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_SWAPCHAIN_FACTORY_HPP
