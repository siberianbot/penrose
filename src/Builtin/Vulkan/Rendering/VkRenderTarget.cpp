#include "VkRenderTarget.hpp"

#include <utility>

#include "src/Rendering/PresentContext.hpp"

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"

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
                                             VkLogicalDeviceContext *logicalDeviceContext,
                                             vk::Image image,
                                             vk::DeviceMemory imageMemory,
                                             vk::ImageView imageView)
            : VkRenderTarget(targetInfo),
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
