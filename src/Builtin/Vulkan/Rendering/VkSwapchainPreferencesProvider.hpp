#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_SWAPCHAIN_PREFERENCES_PROVIDER_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_SWAPCHAIN_PREFERENCES_PROVIDER_HPP

#include <cstdint>
#include <tuple>
#include <vector>

#include <vulkan/vulkan.hpp>

#include <Penrose/Common/Size.hpp>
#include <Penrose/Core/Log.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>

#include "src/Builtin/Vulkan/Rendering/VkPhysicalDeviceContext.hpp"

namespace Penrose {

    class ResourceSet;
    class Surface;

    struct SwapchainPreference {
        vk::Extent2D extent;
        vk::Format format;
        vk::ColorSpaceKHR colorSpace;
        std::uint32_t imageCount;
        vk::PresentModeKHR presentMode;
    };

    class VkSwapchainPreferencesProvider : public Resource {
    public:
        explicit VkSwapchainPreferencesProvider(ResourceSet *resources);
        ~VkSwapchainPreferencesProvider() override = default;

        [[nodiscard]] SwapchainPreference getPreferencesFor(Surface *surface,
                                                            const vk::SurfaceKHR &vkSurface);

    private:
        Lazy<Log> _log;
        Lazy<VkPhysicalDeviceContext> _physicalDeviceContext;

        [[nodiscard]] static vk::Extent2D getPreferredExtent(const vk::SurfaceCapabilitiesKHR &capabilities,
                                                             const Size &surfaceSize);
        [[nodiscard]] static std::uint32_t getPreferredImageCount(const vk::SurfaceCapabilitiesKHR &capabilities);
        [[nodiscard]] static std::tuple<vk::Format, vk::ColorSpaceKHR> getPreferredSurfaceFormat(
                const std::vector<vk::SurfaceFormatKHR> &surfaceFormats);
        [[nodiscard]] static vk::PresentModeKHR getPreferredPresentMode();
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_SWAPCHAIN_PREFERENCES_PROVIDER_HPP
