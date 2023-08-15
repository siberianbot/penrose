#include "VkImage.hpp"

#include "src/Rendering/DeviceContext.hpp"

namespace Penrose {

    VkImage::VkImage(DeviceContext *deviceContext,
                     ImageFormat format, std::uint32_t width, std::uint32_t height,
                     vk::Image image, vk::DeviceMemory imageMemory, vk::ImageView imageView)
            : _deviceContext(deviceContext),
              _format(format),
              _width(width),
              _height(height),
              _image(image),
              _imageMemory(imageMemory),
              _imageView(imageView) {
        //
    }

    VkImage::~VkImage() {
        this->_deviceContext->getLogicalDevice().destroy(this->_imageView);
        this->_deviceContext->getLogicalDevice().free(this->_imageMemory);
        this->_deviceContext->getLogicalDevice().destroy(this->_image);
    }
}
