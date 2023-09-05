#include "VkRenderTarget.hpp"

#include <utility>

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSwapchainManager.hpp"

namespace Penrose {

    VkRenderTarget::VkRenderTarget(RenderTargetInfo &&targetInfo)
            : _targetInfo(targetInfo) {
        //
    }

    VkSwapchainRenderTarget::VkSwapchainRenderTarget(RenderTargetInfo &&targetInfo,
                                                     VkSwapchainManager *swapchainManager)
            : VkRenderTarget(std::forward<decltype(targetInfo)>(targetInfo)),
              _swapchainManager(swapchainManager) {
        //
    }

    const vk::ImageView &VkSwapchainRenderTarget::getView(std::uint32_t imageIdx) const {
        return this->_swapchainManager->getSwapchain()->getImageView(imageIdx);
    }

    VkImageRenderTarget::VkImageRenderTarget(RenderTargetInfo &&targetInfo,
                                             VkLogicalDeviceContext *logicalDeviceContext,
                                             vk::Image image,
                                             vk::DeviceMemory imageMemory,
                                             vk::ImageView imageView)
            : VkRenderTarget(std::forward<decltype(targetInfo)>(targetInfo)),
              _logicalDeviceContext(logicalDeviceContext),
              _image(image),
              _imageMemory(imageMemory),
              _imageView(imageView) {
        //
    }

    VkImageRenderTarget::~VkImageRenderTarget() {
        this->_logicalDeviceContext->getHandle().destroy(this->_imageView);
        this->_logicalDeviceContext->getHandle().free(this->_imageMemory);
        this->_logicalDeviceContext->getHandle().destroy(this->_image);
    }
}
