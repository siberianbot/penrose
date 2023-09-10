#include "VkRenderTargetFactory.hpp"

#include <utility>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

#include "src/Builtin/Vulkan/Rendering/VkUtils.hpp"

namespace Penrose {

    VkRenderTargetFactory::VkRenderTargetFactory(ResourceSet *resources)
            : _logicalDeviceContext(resources->getLazy<VkLogicalDeviceContext>()),
              _memoryAllocator(resources->getLazy<VkMemoryAllocator>()),
              _swapchainManager(resources->getLazy<VkSwapchainManager>()) {
        //
    }

    VkRenderTarget *VkRenderTargetFactory::makeRenderTarget(RenderTargetInfo &&targetInfo) {
        switch (targetInfo.getSource()) {
            case RenderTargetSource::None:
                throw EngineError("Render target source should be either swapchain or image");

            case RenderTargetSource::Image:
                return this->makeImageRenderTarget(std::forward<decltype(targetInfo)>(targetInfo));

            case RenderTargetSource::Swapchain:
                return this->makeSwapchainRenderTarget(std::forward<decltype(targetInfo)>(targetInfo));

            default:
                throw EngineError("Not supported render target source");
        }
    }

    VkRenderTarget *VkRenderTargetFactory::makeImageRenderTarget(RenderTargetInfo &&targetInfo) {
        auto swapchain = this->_swapchainManager->getSwapchain();

        auto format = toVkFormat(targetInfo.getFormat()).value_or(swapchain->getFormat());
        auto extent = map(targetInfo.getSize(), [](Size size) {
            auto [w, h] = size;
            return vk::Extent3D(w, h, 1);
        }).value_or(vk::Extent3D(swapchain->getExtent(), 1));

        auto imageCreateInfo = vk::ImageCreateInfo()
                .setImageType(vk::ImageType::e2D)
                .setUsage(toVkImageUsageFlags(targetInfo.getType()))
                .setExtent(extent)
                .setFormat(format)
                .setMipLevels(1)
                .setArrayLayers(1);
        auto image = this->_logicalDeviceContext->getHandle().createImage(imageCreateInfo);
        auto imageMemory = this->_memoryAllocator->allocate(image);

        auto subresourceRange = vk::ImageSubresourceRange()
                .setBaseMipLevel(0)
                .setLevelCount(1)
                .setBaseArrayLayer(0)
                .setLayerCount(1)
                .setAspectMask(toVkImageAspectFlags(targetInfo.getType()));

        auto imageViewCreateInfo = vk::ImageViewCreateInfo()
                .setImage(image)
                .setViewType(vk::ImageViewType::e2D)
                .setFormat(format)
                .setSubresourceRange(subresourceRange);

        auto imageView = this->_logicalDeviceContext->getHandle().createImageView(imageViewCreateInfo);

        return new VkImageRenderTarget(std::forward<decltype(targetInfo)>(targetInfo),
                                       this->_logicalDeviceContext.get(),
                                       image, imageMemory, imageView);
    }

    VkRenderTarget *VkRenderTargetFactory::makeSwapchainRenderTarget(RenderTargetInfo &&targetInfo) {
        return new VkSwapchainRenderTarget(std::forward<decltype(targetInfo)>(targetInfo),
                                           this->_swapchainManager.get());
    }
}
