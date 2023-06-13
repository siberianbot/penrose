#include "Utils.hpp"

#include "src/Rendering/DeviceContext.hpp"

namespace Penrose {

    DescriptorSetLayout makeDescriptorSetLayout(DeviceContext *deviceContext,
                                                vk::DescriptorSetLayoutCreateInfo createInfo) {
        auto descriptorSetLayout = deviceContext->getLogicalDevice().createDescriptorSetLayout(createInfo);

        return {
                descriptorSetLayout,
                [deviceContext](vk::DescriptorSetLayout &instance) {
                    deviceContext->getLogicalDevice().destroy(instance);
                }
        };
    }

    Pipeline makeGraphicsPipeline(DeviceContext *deviceContext, vk::GraphicsPipelineCreateInfo createInfo) {
        auto pipeline = deviceContext->getLogicalDevice().createGraphicsPipeline(VK_NULL_HANDLE, createInfo);

        return {
                pipeline.value,
                [deviceContext](vk::Pipeline &instance) {
                    deviceContext->getLogicalDevice().destroy(instance);
                }
        };
    }

    PipelineLayout makePipelineLayout(DeviceContext *deviceContext,
                                      vk::PipelineLayoutCreateInfo createInfo) {
        auto pipelineLayout = deviceContext->getLogicalDevice().createPipelineLayout(createInfo);

        return {
                pipelineLayout,
                [deviceContext](vk::PipelineLayout &instance) {
                    deviceContext->getLogicalDevice().destroy(instance);
                }
        };
    }

    ShaderModule makeShaderModule(DeviceContext *deviceContext,
                                  vk::ShaderModuleCreateInfo createInfo) {
        auto shaderModule = deviceContext->getLogicalDevice().createShaderModule(createInfo);

        return {
                shaderModule,
                [deviceContext](vk::ShaderModule &instance) {
                    deviceContext->getLogicalDevice().destroy(instance);
                }
        };
    }
}
