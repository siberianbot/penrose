#ifndef PENROSE_BUILTIN_ASSETS_VK_IMAGE_ASSET_HPP
#define PENROSE_BUILTIN_ASSETS_VK_IMAGE_ASSET_HPP

#include <cstdint>

#include <vulkan/vulkan.hpp>

#include <Penrose/Assets/Asset.hpp>

namespace Penrose {

    class DeviceContext;

    class VkImageAsset : public ImageAsset {
    public:
        VkImageAsset(DeviceContext *deviceContext,
                     vk::Image image, vk::DeviceMemory imageMemory, vk::ImageView imageView);
        ~VkImageAsset() override;

        [[nodiscard]] const vk::Image &getImage() const { return this->_image; }

        [[nodiscard]] const vk::ImageView &getImageView() const { return this->_imageView; }

    private:
        DeviceContext *_deviceContext;
        vk::Image _image;
        vk::DeviceMemory _imageMemory;
        vk::ImageView _imageView;
    };

    [[nodiscard]] VkImageAsset *makeVkImageAsset(DeviceContext *deviceContext,
                                                 std::uint32_t width, std::uint32_t height, std::uint32_t channels,
                                                 void *data);
}

#endif // PENROSE_BUILTIN_ASSETS_VK_IMAGE_ASSET_HPP
