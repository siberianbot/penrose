#ifndef PENROSE_RENDERING_UTILS_HPP
#define PENROSE_RENDERING_UTILS_HPP

#include <vulkan/vulkan.hpp>

#include "src/Utils/DestructibleObject.hpp"

namespace Penrose {

    class DeviceContext;

    using Buffer = DestructibleObject<vk::Buffer>;
    using DescriptorSetLayout = DestructibleObject<vk::DescriptorSetLayout>;
    using DeviceMemory = DestructibleObject<vk::DeviceMemory>;
    using Image = DestructibleObject<vk::Image>;
    using ImageView = DestructibleObject<vk::ImageView>;
    using Pipeline = DestructibleObject<vk::Pipeline>;
    using PipelineLayout = DestructibleObject<vk::PipelineLayout>;
    using ShaderModule = DestructibleObject<vk::ShaderModule>;

    [[nodiscard]] Buffer makeBuffer(DeviceContext *deviceContext,
                                    vk::BufferCreateInfo createInfo);

    [[nodiscard]] DescriptorSetLayout makeDescriptorSetLayout(DeviceContext *deviceContext,
                                                              vk::DescriptorSetLayoutCreateInfo createInfo);

    [[nodiscard]] Image makeImage(DeviceContext *deviceContext,
                                  vk::ImageCreateInfo createInfo);

    [[nodiscard]] ImageView makeImageView(DeviceContext *deviceContext,
                                          vk::ImageViewCreateInfo createInfo);

    [[nodiscard]] Pipeline makeGraphicsPipeline(DeviceContext *deviceContext,
                                                vk::GraphicsPipelineCreateInfo createInfo);

    [[nodiscard]] PipelineLayout makePipelineLayout(DeviceContext *deviceContext,
                                                    vk::PipelineLayoutCreateInfo createInfo);

    [[nodiscard]] ShaderModule makeShaderModule(DeviceContext *deviceContext,
                                                vk::ShaderModuleCreateInfo createInfo);

    [[nodiscard]] DeviceMemory makeDeviceMemory(DeviceContext *deviceContext, const Image &image, bool local = true);
}

#endif // PENROSE_RENDERING_UTILS_HPP
