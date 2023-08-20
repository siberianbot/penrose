#include "VkRenderTarget.hpp"

#include <utility>

#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/PresentContext.hpp"

namespace Penrose {

    VkRenderTarget::VkRenderTarget(RenderTargetInfo targetInfo)
            : _targetInfo(std::move(targetInfo)) {
        //
    }

    VkSwapchainRenderTarget::VkSwapchainRenderTarget(RenderTargetInfo targetInfo,
                                                     PresentContext *presentContext)
            : VkRenderTarget(targetInfo),
              _presentContext(presentContext) {
        //
    }

    const vk::ImageView &VkSwapchainRenderTarget::getView(std::uint32_t imageIdx) const {
        return this->_presentContext->getSwapchainImageViews().at(imageIdx);
    }

    VkImageRenderTarget::VkImageRenderTarget(RenderTargetInfo targetInfo,
                                             DeviceContext *deviceContext,
                                             vk::Image image,
                                             vk::DeviceMemory imageMemory,
                                             vk::ImageView imageView)
            : VkRenderTarget(targetInfo),
              _deviceContext(deviceContext),
              _image(image),
              _imageMemory(imageMemory),
              _imageView(imageView) {
        //
    }

    VkImageRenderTarget::~VkImageRenderTarget() {
        this->_deviceContext->getLogicalDevice().destroy(this->_imageView);
        this->_deviceContext->getLogicalDevice().free(this->_imageMemory);
        this->_deviceContext->getLogicalDevice().destroy(this->_image);
    }
}
