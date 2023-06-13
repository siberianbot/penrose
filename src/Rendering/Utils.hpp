#ifndef PENROSE_RENDERING_UTILS_HPP
#define PENROSE_RENDERING_UTILS_HPP

#include <vulkan/vulkan.hpp>

#include "src/Utils/DestructibleObject.hpp"

namespace Penrose {

    class DeviceContext;

    using DescriptorSetLayout = DestructibleObject<vk::DescriptorSetLayout>;
    using Pipeline = DestructibleObject<vk::Pipeline>;
    using PipelineLayout = DestructibleObject<vk::PipelineLayout>;
    using ShaderModule = DestructibleObject<vk::ShaderModule>;

    [[nodiscard]] DescriptorSetLayout makeDescriptorSetLayout(DeviceContext *deviceContext,
                                                              vk::DescriptorSetLayoutCreateInfo createInfo);
    [[nodiscard]] Pipeline makeGraphicsPipeline(DeviceContext *deviceContext,
                                                vk::GraphicsPipelineCreateInfo createInfo);
    [[nodiscard]] PipelineLayout makePipelineLayout(DeviceContext *deviceContext,
                                                    vk::PipelineLayoutCreateInfo createInfo);
    [[nodiscard]] ShaderModule makeShaderModule(DeviceContext *deviceContext,
                                                vk::ShaderModuleCreateInfo createInfo);
}

#endif // PENROSE_RENDERING_UTILS_HPP
