#include "PresentContext.hpp"

#include <limits>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    vk::Extent2D getPreferredExtent(const vk::SurfaceCapabilitiesKHR &capabilities, const Size &surfaceSize) {
        std::uint32_t maxValue = std::numeric_limits<std::uint32_t>::max();

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

    std::uint32_t getPreferredImageCount(const vk::SurfaceCapabilitiesKHR &capabilities) {
        return capabilities.maxImageCount > 0
               ? std::clamp(capabilities.minImageCount + 1, capabilities.minImageCount, capabilities.maxImageCount)
               : capabilities.minImageCount + 1;
    }

    std::tuple<vk::Format, vk::ColorSpaceKHR> getPreferredSurfaceFormat(
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

    vk::PresentModeKHR getPreferredPresentMode() {
        // TODO
        return vk::PresentModeKHR::eImmediate;
    }

    PresentContext::PresentContext(ResourceSet *resources)
            : _logicalDeviceContext(resources->getLazy<VkLogicalDeviceContext>()),
              _physicalDeviceContext(resources->getLazy<VkPhysicalDeviceContext>()),
              _surfaceManager(resources->getLazy<SurfaceManager>()),
              _surfaceProvider(resources->getLazy<VkSurfaceProvider>()) {
        //
    }

    void PresentContext::init() {
        try {
            this->_swapchain = this->createSwapchain();
        } catch (...) {
            std::throw_with_nested(EngineError("Failed to create swapchain"));
        }
    }

    void PresentContext::destroy() {
        if (this->_swapchain.has_value()) {
            this->destroySwapchain(this->_swapchain.value());
            this->_swapchain = std::nullopt;
        }
    }

    void PresentContext::recreate() {
        auto newSwapchain = this->createSwapchain();

        if (this->_swapchain.has_value()) {
            this->destroySwapchain(this->_swapchain.value());
        }

        this->_swapchain = newSwapchain;
    }

    PresentContext::SwapchainProxy PresentContext::createSwapchain() {
        auto surface = this->_surfaceManager->getSurface();
        auto vkSurface = this->_surfaceProvider->getVkSurfaceFor(surface);

        auto physicalDevice = this->_physicalDeviceContext->getHandle();
        auto logicalDevice = this->_logicalDeviceContext->getHandle();

        auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(vkSurface);
        auto surfaceFormats = physicalDevice.getSurfaceFormatsKHR(vkSurface);

        auto extent = getPreferredExtent(surfaceCapabilities, surface->getSize());
        auto [format, colorSpace] = getPreferredSurfaceFormat(surfaceFormats);
        auto imageCount = getPreferredImageCount(surfaceCapabilities);
        auto presentMode = getPreferredPresentMode();

        auto createInfo = vk::SwapchainCreateInfoKHR()
                .setSurface(vkSurface)
                .setPresentMode(presentMode)
                .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
                .setImageArrayLayers(1)
                .setImageExtent(extent)
                .setImageFormat(format)
                .setImageColorSpace(colorSpace)
                .setMinImageCount(imageCount);

        if (this->_swapchain.has_value()) {
            createInfo.setOldSwapchain(this->_swapchain.value().handle);
        }

        auto swapchain = logicalDevice.createSwapchainKHR(createInfo);
        auto images = logicalDevice.getSwapchainImagesKHR(swapchain);

        auto imageSubresourceRange = vk::ImageSubresourceRange()
                .setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setBaseMipLevel(0)
                .setLevelCount(1)
                .setBaseArrayLayer(0)
                .setLayerCount(1);

        auto imageViewCreateInfo = vk::ImageViewCreateInfo()
                .setFormat(format)
                .setViewType(vk::ImageViewType::e2D)
                .setSubresourceRange(imageSubresourceRange);

        std::vector<vk::ImageView> imageViews;
        for (const auto &image: images) {
            imageViewCreateInfo.setImage(image);

            auto imageView = logicalDevice.createImageView(imageViewCreateInfo);

            imageViews.push_back(imageView);
        }

        return {
                .handle = swapchain,
                .extent = extent,
                .format = format,
                .images = images,
                .imageViews = imageViews
        };
    }

    void PresentContext::destroySwapchain(const PresentContext::SwapchainProxy &swapchain) {
        auto logicalDevice = this->_logicalDeviceContext->getHandle();

        for (const auto &imageView: swapchain.imageViews) {
            logicalDevice.destroy(imageView);
        }

        logicalDevice.destroy(swapchain.handle);
    }
}
