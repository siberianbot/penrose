#include "VkSurfacePreferencesProvider.hpp"

#include <algorithm>
#include <limits>
#include <string_view>

namespace Penrose {

    inline static constexpr std::string_view TAG = "VkSurfacePreferencesProvider";

    VkSurfacePreferencesProvider::VkSurfacePreferencesProvider(const ResourceSet *resources)
        : _log(resources->get<Log>()),
          _physicalDeviceProvider(resources->get<VkPhysicalDeviceProvider>()) {
        //
    }

    SurfacePreferences VkSurfacePreferencesProvider::getPreferencesFor(
        const std::shared_ptr<Surface> &surface, const vk::SurfaceKHR &vkSurface
    ) {
        auto &physicalDevice = this->_physicalDeviceProvider->getPhysicalDevice().handle;

        const auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(vkSurface);
        const auto surfaceFormats = physicalDevice.getSurfaceFormatsKHR(vkSurface);
        const auto [width, height] = surface->getSize();

        auto extent = getPreferredExtent(surfaceCapabilities, width, height);
        auto [format, colorSpace] = getPreferredSurfaceFormat(surfaceFormats);
        auto imageCount = getPreferredImageCount(surfaceCapabilities);
        auto presentMode = getPreferredPresentMode();

        this->_log->writeDebug(
            TAG,
            "Surface {:#x} preferences: extent = {}x{}, format = {:#x}, color space = {:#x}, "
            "image count = {}, present mode = {:#x}",
            reinterpret_cast<std::size_t>(surface.get()), extent.width, extent.height,
            static_cast<std::uint32_t>(format), static_cast<std::uint32_t>(colorSpace), imageCount,
            static_cast<std::uint32_t>(presentMode)
        );

        return {
            .extent = extent,
            .format = format,
            .colorSpace = colorSpace,
            .imageCount = imageCount,
            .presentMode = presentMode
        };
    }

    vk::Extent2D VkSurfacePreferencesProvider::getPreferredExtent(
        const vk::SurfaceCapabilitiesKHR &capabilities, const std::uint32_t width, const std::uint32_t height
    ) {
        constexpr auto maxValue = std::numeric_limits<std::uint32_t>::max();

        if (capabilities.currentExtent.width != maxValue && capabilities.currentExtent.height != maxValue) {
            return capabilities.currentExtent;
        }

        return vk::Extent2D()
            .setWidth(std::clamp(width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width))
            .setHeight(std::clamp(height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height));
    }

    std::uint32_t VkSurfacePreferencesProvider::getPreferredImageCount(const vk::SurfaceCapabilitiesKHR &capabilities) {
        return capabilities.maxImageCount > 0
                   ? std::clamp(capabilities.minImageCount + 1, capabilities.minImageCount, capabilities.maxImageCount)
                   : capabilities.minImageCount + 1;
    }

    std::tuple<vk::Format, vk::ColorSpaceKHR> VkSurfacePreferencesProvider::getPreferredSurfaceFormat(
        const std::vector<vk::SurfaceFormatKHR> &surfaceFormats
    ) {
        const auto it = std::ranges::find_if(surfaceFormats, [](const vk::SurfaceFormatKHR &surfaceFormat) {
            return surfaceFormat.format == vk::Format::eR8G8B8A8Srgb
                   && surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
        });

        if (it == surfaceFormats.end()) {
            return std::make_tuple(surfaceFormats[0].format, surfaceFormats[0].colorSpace);
        }

        return std::make_tuple(it->format, it->colorSpace);
    }

    vk::PresentModeKHR VkSurfacePreferencesProvider::getPreferredPresentMode() {
        // TODO
        return vk::PresentModeKHR::eImmediate;
    }
}
