#include "VkRenderTarget.hpp"

#include <utility>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/PresentContext.hpp"

#include "src/Builtin/Rendering/VkUtils.hpp"

namespace Penrose {

    VkRenderTarget::VkRenderTarget(RenderTargetInfo target)
            : _target(std::move(target)) {
        //
    }

    VkSwapchainRenderTarget::VkSwapchainRenderTarget(RenderTargetInfo target,
                                                     PresentContext *presentContext)
            : VkRenderTarget(target),
              _presentContext(presentContext) {
        //
    }

    const vk::ImageView &VkSwapchainRenderTarget::getView(std::uint32_t imageIdx) const {
        return this->_presentContext->getSwapchainImageViews().at(imageIdx);
    }

    VkImageRenderTarget::VkImageRenderTarget(RenderTargetInfo target,
                                             DeviceContext *deviceContext,
                                             PresentContext *presentContext)
            : VkRenderTarget(target),
              _deviceContext(deviceContext),
              _presentContext(presentContext) {
        //
    }

    void VkImageRenderTarget::create() {
        auto format = toVkFormat(this->_target.getFormat()).value_or(this->_presentContext->getSwapchainFormat());
        auto extent = map(this->_target.getSize(), [](Size size) {
            auto [w, h] = size;
            return vk::Extent3D(w, h, 1);
        }).value_or(vk::Extent3D(this->_presentContext->getSwapchainExtent(), 1));

        auto imageCreateInfo = vk::ImageCreateInfo()
                .setImageType(vk::ImageType::e2D)
                .setUsage(toVkImageUsageFlags(this->_target.getType()))
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
                .setAspectMask(toVkImageAspectFlags(this->_target.getType()));

        auto imageViewCreateInfo = vk::ImageViewCreateInfo()
                .setImage(image)
                .setViewType(vk::ImageViewType::e2D)
                .setFormat(format)
                .setSubresourceRange(subresourceRange);

        auto imageView = this->_deviceContext->getLogicalDevice().createImageView(imageViewCreateInfo);

        this->_state = State{
                .image = image,
                .imageMemory = imageMemory,
                .imageView = imageView
        };
    }

    void VkImageRenderTarget::destroy() {
        if (!this->_state.has_value()) {
            return;
        }

        this->_deviceContext->getLogicalDevice().destroy(this->_state->imageView);
        this->_deviceContext->getLogicalDevice().free(this->_state->imageMemory);
        this->_deviceContext->getLogicalDevice().destroy(this->_state->image);

        this->_state = std::nullopt;
    }

    VkRenderTarget *makeVkRenderTarget(DeviceContext *deviceContext,
                                       PresentContext *presentContext,
                                       const RenderTargetInfo &target) {
        switch (target.getSource()) {
            case RenderTargetSource::None:
                throw EngineError("Render target source should be either swapchain or image");

            case RenderTargetSource::Image:
                return new VkImageRenderTarget(target, deviceContext, presentContext);

            case RenderTargetSource::Swapchain:
                return new VkSwapchainRenderTarget(target, presentContext);

            default:
                throw EngineError("Not supported render target source");
        }
    }
}
