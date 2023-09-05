#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_TARGET_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_TARGET_HPP

#include <cstdint>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/RenderTarget.hpp>

namespace Penrose {

    class VkLogicalDeviceContext;
    class VkSwapchainManager;

    class VkRenderTarget : public RenderTarget {
    public:
        explicit VkRenderTarget(RenderTargetInfo &&targetInfo);
        ~VkRenderTarget() override = default;

        [[nodiscard]] const RenderTargetInfo &getTargetInfo() const override { return this->_targetInfo; }

        [[nodiscard]] virtual const vk::ImageView &getView(std::uint32_t imageIdx) const = 0;

    protected:
        RenderTargetInfo _targetInfo;
    };

    class VkSwapchainRenderTarget : public VkRenderTarget {
    public:
        explicit VkSwapchainRenderTarget(RenderTargetInfo &&targetInfo,
                                         VkSwapchainManager *swapchainManager);
        ~VkSwapchainRenderTarget() override = default;

        [[nodiscard]] const vk::ImageView &getView(std::uint32_t imageIdx) const override;

    private:
        VkSwapchainManager *_swapchainManager;
    };

    class VkImageRenderTarget : public VkRenderTarget {
    public:
        VkImageRenderTarget(RenderTargetInfo &&targetInfo,
                            VkLogicalDeviceContext *logicalDeviceContext,
                            vk::Image image,
                            vk::DeviceMemory imageMemory,
                            vk::ImageView imageView);
        ~VkImageRenderTarget() override;

        [[nodiscard]] const vk::ImageView &getView(std::uint32_t) const override { return this->_imageView; }

    private:
        VkLogicalDeviceContext *_logicalDeviceContext;

        vk::Image _image;
        vk::DeviceMemory _imageMemory;
        vk::ImageView _imageView;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_RENDER_TARGET_HPP
