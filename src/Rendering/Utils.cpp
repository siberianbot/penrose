#include <Penrose/Rendering/Utils.hpp>

#include <optional>

#include <Penrose/Common/EngineError.hpp>

#include "src/Rendering/DeviceContext.hpp"

namespace Penrose {

    Buffer makeBuffer(DeviceContext *deviceContext, vk::BufferCreateInfo createInfo) {
        auto buffer = deviceContext->getLogicalDevice().createBuffer(createInfo);

        return {
                buffer,
                [deviceContext](vk::Buffer &instance) {
                    deviceContext->getLogicalDevice().destroy(instance);
                }
        };
    }

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

    Image makeImage(DeviceContext *deviceContext, vk::ImageCreateInfo createInfo) {
        auto image = deviceContext->getLogicalDevice().createImage(createInfo);

        return {
                image,
                [deviceContext](vk::Image &instance) {
                    deviceContext->getLogicalDevice().destroy(instance);
                }
        };
    }

    ImageView makeImageView(DeviceContext *deviceContext, vk::ImageViewCreateInfo createInfo) {
        auto imageView = deviceContext->getLogicalDevice().createImageView(createInfo);

        return {
                imageView,
                [deviceContext](vk::ImageView &instance) {
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

    Sampler makeSampler(DeviceContext *deviceContext, vk::SamplerCreateInfo createInfo) {
        auto sampler = deviceContext->getLogicalDevice().createSampler(createInfo);

        return {
                sampler,
                [deviceContext](vk::Sampler &instance) {
                    deviceContext->getLogicalDevice().destroy(instance);
                }
        };
    }
}
