#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_SURFACE_PREFERENCES_PROVIDER_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_SURFACE_PREFERENCES_PROVIDER_HPP

#include <cstdint>
#include <memory>
#include <tuple>
#include <vector>

#include <vulkan/vulkan.hpp>

#include <Penrose/Common/Log.hpp>
#include <Penrose/Rendering/Surface.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Builtin/Vulkan/Rendering/Objects/VkPhysicalDeviceProvider.hpp"

namespace Penrose {

    struct SurfacePreferences {
        vk::Extent2D extent;
        vk::Format format;
        vk::ColorSpaceKHR colorSpace;
        std::uint32_t imageCount;
        vk::PresentModeKHR presentMode;
    };

    class VkSurfacePreferencesProvider final: public Resource<VkSurfacePreferencesProvider> {
    public:
        explicit VkSurfacePreferencesProvider(const ResourceSet *resources);
        ~VkSurfacePreferencesProvider() override = default;

        [[nodiscard]] SurfacePreferences getPreferencesFor(
            const std::shared_ptr<Surface> &surface, const vk::SurfaceKHR &vkSurface
        );

    private:
        ResourceProxy<Log> _log;
        ResourceProxy<VkPhysicalDeviceProvider> _physicalDeviceProvider;

        [[nodiscard]] static vk::Extent2D getPreferredExtent(
            const vk::SurfaceCapabilitiesKHR &capabilities, std::uint32_t width, std::uint32_t height
        );

        [[nodiscard]] static std::uint32_t getPreferredImageCount(const vk::SurfaceCapabilitiesKHR &capabilities);

        [[nodiscard]] static std::tuple<vk::Format, vk::ColorSpaceKHR> getPreferredSurfaceFormat(
            const std::vector<vk::SurfaceFormatKHR> &surfaceFormats
        );

        [[nodiscard]] static vk::PresentModeKHR getPreferredPresentMode();
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_SURFACE_PREFERENCES_PROVIDER_HPP
