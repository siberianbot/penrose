#include "VkSwapchain.hpp"

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"

namespace Penrose {

    VkSwapchain::VkSwapchain(VkLogicalDeviceContext *logicalDeviceContext,
                             vk::SwapchainKHR handle,
                             vk::Extent2D extent, vk::Format format,
                             std::vector<vk::ImageView> &&imageViews)
            : _logicalDeviceContext(logicalDeviceContext),
              _handle(handle),
              _extent(extent),
              _format(format),
              _imageViews(imageViews) {
        //
    }

    VkSwapchain::~VkSwapchain() {
        for (const auto &imageView: this->_imageViews) {
            this->_logicalDeviceContext->getHandle().destroy(imageView);
        }

        this->_logicalDeviceContext->getHandle().destroy(this->_handle);
    }
}
