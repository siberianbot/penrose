#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_SWAPCHAIN_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_SWAPCHAIN_HPP

#include <cstdint>

#include <vulkan/vulkan.hpp>

namespace Penrose {

    class VkLogicalDeviceContext;

    class VkSwapchain {
    public:
        VkSwapchain(VkLogicalDeviceContext *logicalDeviceContext,
                    vk::SwapchainKHR handle,
                    vk::Extent2D extent, vk::Format format,
                    std::vector<vk::ImageView> &&imageViews);
        ~VkSwapchain();

        [[nodiscard]] const vk::SwapchainKHR &getHandle() const { return this->_handle; }

        [[nodiscard]] const vk::Extent2D &getExtent() const { return this->_extent; }

        [[nodiscard]] const vk::Format &getFormat() const { return this->_format; }

        [[nodiscard]] std::uint32_t getImageCount() const { return this->_imageViews.size(); }

        [[nodiscard]] const vk::ImageView &getImageView(std::uint32_t imageIdx) const {
            return this->_imageViews.at(imageIdx);
        }

    private:
        VkLogicalDeviceContext *_logicalDeviceContext;

        vk::SwapchainKHR _handle;
        vk::Extent2D _extent;
        vk::Format _format;
        std::vector<vk::ImageView> _imageViews;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_SWAPCHAIN_HPP
