#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_UTILS_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_UTILS_HPP

#include <cstdint>
#include <optional>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/Buffer.hpp>
#include <Penrose/Rendering/Image.hpp>
#include <Penrose/Rendering/PipelineInfo.hpp>
#include <Penrose/Rendering/RenderGraphInfo.hpp>
#include <Penrose/Rendering/SamplerInfo.hpp>

namespace Penrose {

    class DeviceContext;

    [[nodiscard]] vk::DescriptorType toVkDescriptorType(const PipelineLayoutBindingType &type);
    [[nodiscard]] vk::ShaderStageFlags toVkShaderStageFlags(const PipelineShaderStageType &type);
    [[nodiscard]] vk::ShaderStageFlagBits toVkShaderStageFlagBits(const PipelineShaderStageType &type);

    [[nodiscard]] vk::VertexInputRate toVkVertexInputRate(const PipelineBindingInputRate &inputRate);

    [[nodiscard]] vk::BufferUsageFlags toVkBufferUsageFlags(const BufferType &type);

    [[nodiscard]] vk::ImageUsageFlags toVkImageUsageFlags(const RenderTargetType &type);
    [[nodiscard]] vk::ImageAspectFlags toVkImageAspectFlags(const RenderTargetType &type);

    [[nodiscard]] vk::Format toVkFormat(const PipelineBindingAttributeFormat &format);
    [[nodiscard]] vk::Format toVkFormat(const ImageFormat &format);
    [[nodiscard]] std::optional<vk::Format> toVkFormat(const RenderFormat &format);

    [[nodiscard]] vk::AttachmentLoadOp toVkAttachmentLoadOp(const RenderAttachmentLoadOp &loadOp);
    [[nodiscard]] vk::AttachmentStoreOp toVkAttachmentStoreOp(const RenderAttachmentStoreOp &storeOp);

    [[nodiscard]] vk::ImageLayout toVkImageLayout(const RenderAttachmentLayout &layout);

    [[nodiscard]] vk::SamplerAddressMode toVkSamplerAddressMode(const SamplerAddressMode &addressMode);
    [[nodiscard]] vk::BorderColor toVkBorderColor(const SamplerBorderColor &borderColor);
    [[nodiscard]] vk::Filter toVkFilter(const SamplerFilter &filter);

    [[nodiscard]] vk::DeviceMemory makeDeviceMemory(DeviceContext *deviceContext, const vk::Buffer &buffer,
                                                    bool local = true);
    [[nodiscard]] vk::DeviceMemory makeDeviceMemory(DeviceContext *deviceContext, const vk::Image &image,
                                                    bool local = true);
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_UTILS_HPP
