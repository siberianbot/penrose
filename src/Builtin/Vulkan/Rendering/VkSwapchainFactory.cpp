#include "VkSwapchainFactory.hpp"

#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    VkSwapchainFactory::VkSwapchainFactory(ResourceSet *resources)
            : _logicalDeviceContext(resources->getLazy<VkLogicalDeviceContext>()),
              _surfaceManager(resources->getLazy<SurfaceManager>()),
              _surfaceProvider(resources->getLazy<VkSurfaceProvider>()),
              _swapchainPreferencesProvider(resources->getLazy<VkSwapchainPreferencesProvider>()) {
        //
    }

    VkSwapchain *VkSwapchainFactory::makeSwapchain(std::optional<VkSwapchain *> oldSwapchain) {
        auto surface = this->_surfaceManager->getSurface();
        auto vkSurface = this->_surfaceProvider->getVkSurfaceFor(surface);

        auto preferences = this->_swapchainPreferencesProvider->getPreferencesFor(surface, vkSurface);

        auto createInfo = vk::SwapchainCreateInfoKHR()
                .setSurface(vkSurface)
                .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
                .setImageArrayLayers(1)
                .setImageExtent(preferences.extent)
                .setImageFormat(preferences.format)
                .setImageColorSpace(preferences.colorSpace)
                .setMinImageCount(preferences.imageCount)
                .setPresentMode(preferences.presentMode);

        if (oldSwapchain.has_value()) {
            createInfo.setOldSwapchain((*oldSwapchain)->getHandle());
        }

        auto swapchain = this->_logicalDeviceContext->getHandle().createSwapchainKHR(createInfo);
        auto images = this->_logicalDeviceContext->getHandle().getSwapchainImagesKHR(swapchain);

        auto imageSubresourceRange = vk::ImageSubresourceRange()
                .setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setBaseMipLevel(0)
                .setLevelCount(1)
                .setBaseArrayLayer(0)
                .setLayerCount(1);

        auto imageViewCreateInfo = vk::ImageViewCreateInfo()
                .setFormat(preferences.format)
                .setViewType(vk::ImageViewType::e2D)
                .setSubresourceRange(imageSubresourceRange);

        std::vector<vk::ImageView> imageViews;
        for (const auto &image: images) {
            imageViewCreateInfo.setImage(image);

            auto imageView = this->_logicalDeviceContext->getHandle().createImageView(imageViewCreateInfo);

            imageViews.push_back(imageView);
        }

        return new VkSwapchain(this->_logicalDeviceContext.get(),
                               swapchain, preferences.extent, preferences.format,
                               std::forward<decltype(imageViews)>(imageViews));
    }
}