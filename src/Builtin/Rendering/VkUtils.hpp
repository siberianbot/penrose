#ifndef PENROSE_BUILTIN_RENDERING_VK_UTILS_HPP
#define PENROSE_BUILTIN_RENDERING_VK_UTILS_HPP

#include <optional>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/Buffer.hpp>
#include <Penrose/Rendering/Image.hpp>
#include <Penrose/Rendering/RenderGraphInfo.hpp>

namespace Penrose {

    class DeviceContext;

    [[nodiscard]] vk::BufferUsageFlags toVkBufferUsageFlags(const BufferType &type);

    [[nodiscard]] vk::ImageUsageFlags toVkImageUsageFlags(const RenderTargetType &type);
    [[nodiscard]] vk::ImageAspectFlags toVkImageAspectFlags(const RenderTargetType &type);

    [[nodiscard]] vk::Format toVkFormat(const ImageFormat &format);
    [[nodiscard]] std::optional<vk::Format> toVkFormat(const RenderFormat &format);

    [[nodiscard]] vk::AttachmentLoadOp toVkAttachmentLoadOp(const RenderAttachmentLoadOp &loadOp);
    [[nodiscard]] vk::AttachmentStoreOp toVkAttachmentStoreOp(const RenderAttachmentStoreOp &storeOp);

    [[nodiscard]] vk::ImageLayout toVkImageLayout(const RenderAttachmentLayout &layout);

    [[nodiscard]] vk::DeviceMemory makeDeviceMemory(DeviceContext *deviceContext, const vk::Buffer &buffer,
                                                    bool local = true);
    [[nodiscard]] vk::DeviceMemory makeDeviceMemory(DeviceContext *deviceContext, const vk::Image &image,
                                                    bool local = true);
}

#endif // PENROSE_BUILTIN_RENDERING_VK_UTILS_HPP
