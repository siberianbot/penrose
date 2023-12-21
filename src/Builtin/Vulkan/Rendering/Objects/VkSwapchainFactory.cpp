#include "VkSwapchainFactory.hpp"

namespace Penrose {

    VkSwapchainFactory::VkSwapchainFactory(const ResourceSet *resources)
        : _logicalDeviceProvider(resources->get<VkLogicalDeviceProvider>()),
          _surfaceProvider(resources->get<VkSurfaceProvider>()),
          _surfacePreferencesProvider(resources->get<VkSurfacePreferencesProvider>()) {
        //
    }

    VkSwapchain VkSwapchainFactory::makeSwapchain(const std::shared_ptr<Surface> &surface) {
        const auto vkSurface = this->_surfaceProvider->getVkSurfaceFor(surface.get());
        const auto preferences = this->_surfacePreferencesProvider->getPreferencesFor(surface, vkSurface);

        const auto swapchainCreateInfo = vk::SwapchainCreateInfoKHR()
                                             .setSurface(vkSurface)
                                             .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
                                             .setImageArrayLayers(1)
                                             .setImageExtent(preferences.extent)
                                             .setImageFormat(preferences.format)
                                             .setImageColorSpace(preferences.colorSpace)
                                             .setMinImageCount(preferences.imageCount)
                                             .setPresentMode(preferences.presentMode);

        return this->makeSwapchain(surface, swapchainCreateInfo);
    }

    VkSwapchain VkSwapchainFactory::makeSwapchain(const VkSwapchain &oldSwapchain) {
        const auto vkSurface = this->_surfaceProvider->getVkSurfaceFor(oldSwapchain.surface.get());
        const auto [extent, format, colorSpace, imageCount, presentMode] = this->_surfacePreferencesProvider
                                                                               ->getPreferencesFor(
                                                                                   oldSwapchain.surface, vkSurface
                                                                               );

        const auto swapchainCreateInfo = vk::SwapchainCreateInfoKHR()
                                             .setSurface(vkSurface)
                                             .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
                                             .setImageArrayLayers(1)
                                             .setImageExtent(extent)
                                             .setImageFormat(format)
                                             .setImageColorSpace(colorSpace)
                                             .setMinImageCount(imageCount)
                                             .setPresentMode(presentMode)
                                             .setOldSwapchain(oldSwapchain.handle.get());

        return this->makeSwapchain(oldSwapchain.surface, swapchainCreateInfo);
    }

    VkSwapchain VkSwapchainFactory::makeSwapchain(
        const std::shared_ptr<Surface> &surface, const vk::SwapchainCreateInfoKHR &swapchainCreateInfo
    ) {
        auto &device = this->_logicalDeviceProvider->getLogicalDevice().handle;

        auto swapchain = device->createSwapchainKHRUnique(swapchainCreateInfo);
        const auto images = device->getSwapchainImagesKHR(swapchain.get());

        constexpr auto imageSubresourceRange = vk::ImageSubresourceRange()
                                                   .setAspectMask(vk::ImageAspectFlagBits::eColor)
                                                   .setBaseMipLevel(0)
                                                   .setLevelCount(1)
                                                   .setBaseArrayLayer(0)
                                                   .setLayerCount(1);

        auto imageViewCreateInfo = vk::ImageViewCreateInfo()
                                       .setFormat(swapchainCreateInfo.imageFormat)
                                       .setViewType(vk::ImageViewType::e2D)
                                       .setSubresourceRange(imageSubresourceRange);

        std::vector<vk::UniqueImageView> imageViews;
        for (const auto &image: images) {
            imageViewCreateInfo.setImage(image);

            imageViews.push_back(device->createImageViewUnique(imageViewCreateInfo));
        }

        return VkSwapchain {
            .surface = surface,
            .extent = swapchainCreateInfo.imageExtent,
            .format = swapchainCreateInfo.imageFormat,
            .handle = std::move(swapchain),
            .imageViews = std::move(imageViews)
        };
    }
}
