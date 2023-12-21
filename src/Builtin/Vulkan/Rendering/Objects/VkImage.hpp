#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_IMAGE_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_IMAGE_HPP

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/Objects/Image.hpp>

namespace Penrose {

    class VkImage final: public Image {
    public:
        VkImage(
            const ImageFormat format, const std::uint32_t width, const std::uint32_t height, vk::UniqueImage &&image,
            vk::UniqueDeviceMemory &&imageMemory, vk::UniqueImageView &&imageView
        )
            : _format(format),
              _width(width),
              _height(height),
              _image(std::forward<decltype(image)>(image)),
              _imageMemory(std::forward<decltype(imageMemory)>(imageMemory)),
              _imageView(std::forward<decltype(imageView)>(imageView)) {
            //
        }

        ~VkImage() override = default;

        [[nodiscard]] ImageFormat getFormat() const override { return this->_format; }

        [[nodiscard]] std::uint32_t getWidth() const override { return this->_width; }

        [[nodiscard]] std::uint32_t getHeight() const override { return this->_height; }

        [[nodiscard]] const vk::Image &getHandle() const { return this->_image.get(); }

        [[nodiscard]] const vk::ImageView &getViewHandle() const { return this->_imageView.get(); }

    private:
        ImageFormat _format;
        std::uint32_t _width;
        std::uint32_t _height;

        vk::UniqueImage _image;
        vk::UniqueDeviceMemory _imageMemory;
        vk::UniqueImageView _imageView;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_IMAGE_HPP
