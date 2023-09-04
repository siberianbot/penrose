#include "VkImage.hpp"

#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"

namespace Penrose {

    VkImage::VkImage(VkLogicalDeviceContext *logicalDeviceContext,
                     ImageFormat format, std::uint32_t width, std::uint32_t height,
                     vk::Image image, vk::DeviceMemory imageMemory, vk::ImageView imageView)
            : _logicalDeviceContext(logicalDeviceContext),
              _format(format),
              _width(width),
              _height(height),
              _image(image),
              _imageMemory(imageMemory),
              _imageView(imageView) {
        //
    }

    VkImage::~VkImage() {
        this->_logicalDeviceContext->getHandle().destroy(this->_imageView);
        this->_logicalDeviceContext->getHandle().free(this->_imageMemory);
        this->_logicalDeviceContext->getHandle().destroy(this->_image);
    }
}
