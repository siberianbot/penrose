#include "VkSwapchainPreferencesProvider.hpp"

#include <algorithm>
#include <limits>
#include <string_view>

namespace Penrose {

    constexpr static const std::string_view VK_SWAPCHAIN_PREFERENCES_PROVIDER_TAG = "VkSwapchainManager";

    VkSwapchainPreferencesProvider::VkSwapchainPreferencesProvider(const ResourceSet *resources)
            : _log(resources->get<Log>()),
              _physicalDeviceContext(resources->get<VkPhysicalDeviceContext>()) {
        //
    }

    SwapchainPreference VkSwapchainPreferencesProvider::getPreferencesFor(Surface *surface,
                                                                          const vk::SurfaceKHR &vkSurface) {
        auto surfaceCapabilities = this->_physicalDeviceContext->getHandle().getSurfaceCapabilitiesKHR(vkSurface);
        auto surfaceFormats = this->_physicalDeviceContext->getHandle().getSurfaceFormatsKHR(vkSurface);

        auto extent = getPreferredExtent(surfaceCapabilities, surface->getSize());
        auto [format, colorSpace] = getPreferredSurfaceFormat(surfaceFormats);
        auto imageCount = getPreferredImageCount(surfaceCapabilities);
        auto presentMode = getPreferredPresentMode();

        this->_log->writeDebug(VK_SWAPCHAIN_PREFERENCES_PROVIDER_TAG,
                               "Surface {:#x} preferences: extent = {}x{}, format = {:#x}, color space = {:#x}, "
                               "image count = {}, present mode = {:#x}",
                               (std::size_t) surface,
                               extent.width, extent.height,
                               (std::uint32_t) format,
                               (std::uint32_t) colorSpace,
                               imageCount,
                               (std::uint32_t) presentMode);

        return {
                .extent = extent,
                .format = format,
                .colorSpace = colorSpace,
                .imageCount = imageCount,
                .presentMode = presentMode
        };
    }

    vk::Extent2D VkSwapchainPreferencesProvider::getPreferredExtent(const vk::SurfaceCapabilitiesKHR &capabilities,
                                                                    const Size &surfaceSize) {
        constexpr auto maxValue = std::numeric_limits<std::uint32_t>::max();

        if (capabilities.currentExtent.width != maxValue && capabilities.currentExtent.height != maxValue) {
            return capabilities.currentExtent;
        }

        auto [width, height] = surfaceSize;

        return vk::Extent2D()
                .setWidth(std::clamp(width,
                                     capabilities.minImageExtent.width,
                                     capabilities.maxImageExtent.width))
                .setHeight(std::clamp(height,
                                      capabilities.minImageExtent.height,
                                      capabilities.maxImageExtent.height));
    }

    std::uint32_t
    VkSwapchainPreferencesProvider::getPreferredImageCount(const vk::SurfaceCapabilitiesKHR &capabilities) {
        return capabilities.maxImageCount > 0
               ? std::clamp(capabilities.minImageCount + 1, capabilities.minImageCount, capabilities.maxImageCount)
               : capabilities.minImageCount + 1;
    }

    std::tuple<vk::Format, vk::ColorSpaceKHR> VkSwapchainPreferencesProvider::getPreferredSurfaceFormat(
            const std::vector<vk::SurfaceFormatKHR> &surfaceFormats) {
        auto it = std::find_if(surfaceFormats.begin(), surfaceFormats.end(),
                               [](const vk::SurfaceFormatKHR &surfaceFormat) {
                                   return surfaceFormat.format == vk::Format::eR8G8B8A8Srgb &&
                                          surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
                               });

        if (it == surfaceFormats.end()) {
            return std::make_tuple(surfaceFormats[0].format, surfaceFormats[0].colorSpace);
        }

        return std::make_tuple(it->format, it->colorSpace);
    }

    vk::PresentModeKHR VkSwapchainPreferencesProvider::getPreferredPresentMode() {
        // TODO
        return vk::PresentModeKHR::eImmediate;
    }
}
