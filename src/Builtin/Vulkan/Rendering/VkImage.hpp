#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_IMAGE_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_IMAGE_HPP

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/Image.hpp>

namespace Penrose {

    class VkLogicalDeviceContext;

    class VkImage : public Image {
    public:
        VkImage(VkLogicalDeviceContext *logicalDeviceContext,
                ImageFormat format, std::uint32_t width, std::uint32_t height,
                vk::Image image, vk::DeviceMemory imageMemory, vk::ImageView imageView);
        ~VkImage() override;

        [[nodiscard]] ImageFormat getFormat() const override { return this->_format; }

        [[nodiscard]] std::uint32_t getWidth() const override { return this->_width; }

        [[nodiscard]] std::uint32_t getHeight() const override { return this->_height; }

        [[nodiscard]] const vk::Image &getImage() const { return this->_image; }

        [[nodiscard]] const vk::ImageView &getImageView() const { return this->_imageView; }

    private:
        VkLogicalDeviceContext *_logicalDeviceContext;

        ImageFormat _format;
        std::uint32_t _width;
        std::uint32_t _height;

        vk::Image _image;
        vk::DeviceMemory _imageMemory;
        vk::ImageView _imageView;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_IMAGE_HPP
