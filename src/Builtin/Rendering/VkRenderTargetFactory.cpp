#include "VkRenderTargetFactory.hpp"

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/PresentContext.hpp"

#include "src/Builtin/Rendering/VkRenderTarget.hpp"
#include "src/Builtin/Rendering/VkUtils.hpp"

namespace Penrose {

    VkRenderTargetFactory::VkRenderTargetFactory(ResourceSet *resources)
            : _deviceContext(resources->get<DeviceContext>()),
              _presentContext(resources->get<PresentContext>()) {
        //
    }

    RenderTarget *VkRenderTargetFactory::makeRenderTarget(const RenderTargetInfo &targetInfo) {
        switch (targetInfo.getSource()) {
            case RenderTargetSource::None:
                throw EngineError("Render target source should be either swapchain or image");

            case RenderTargetSource::Image:
                return this->makeImageRenderTarget(targetInfo);

            case RenderTargetSource::Swapchain:
                return this->makeSwapchainRenderTarget(targetInfo);

            default:
                throw EngineError("Not supported render target source");
        }
    }

    RenderTarget *VkRenderTargetFactory::makeImageRenderTarget(const RenderTargetInfo &targetInfo) {
        auto format = toVkFormat(targetInfo.getFormat()).value_or(this->_presentContext->getSwapchainFormat());
        auto extent = map(targetInfo.getSize(), [](Size size) {
            auto [w, h] = size;
            return vk::Extent3D(w, h, 1);
        }).value_or(vk::Extent3D(this->_presentContext->getSwapchainExtent(), 1));

        auto imageCreateInfo = vk::ImageCreateInfo()
                .setImageType(vk::ImageType::e2D)
                .setUsage(toVkImageUsageFlags(targetInfo.getType()))
                .setExtent(extent)
                .setFormat(format)
                .setMipLevels(1)
                .setArrayLayers(1);
        auto image = this->_deviceContext->getLogicalDevice().createImage(imageCreateInfo);
        auto imageMemory = makeDeviceMemory(this->_deviceContext, image);

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

        auto imageView = this->_deviceContext->getLogicalDevice().createImageView(imageViewCreateInfo);

        return new VkImageRenderTarget(targetInfo, this->_deviceContext, image, imageMemory, imageView);
    }

    RenderTarget *VkRenderTargetFactory::makeSwapchainRenderTarget(const RenderTargetInfo &targetInfo) {
        return new VkSwapchainRenderTarget(targetInfo, this->_presentContext);
    }
}
