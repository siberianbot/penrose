#include "Utils.hpp"

#include <optional>

#include "src/Common/EngineError.hpp"
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

    DeviceMemory makeDeviceMemory(DeviceContext *deviceContext, const vk::MemoryRequirements requirements, bool local) {
        vk::MemoryPropertyFlags flags = local
                                        ? vk::MemoryPropertyFlagBits::eDeviceLocal
                                        : vk::MemoryPropertyFlagBits::eHostVisible |
                                          vk::MemoryPropertyFlagBits::eHostCoherent;

        auto properties = deviceContext->getPhysicalDevice().getMemoryProperties();
        std::optional<uint32_t> memTypeIdx;

        for (uint32_t idx = 0; idx < properties.memoryTypeCount; idx++) {
            bool typeMatches = requirements.memoryTypeBits & (1 << idx);
            bool propertiesMatches = (properties.memoryTypes[idx].propertyFlags & flags) == flags;

            if (!typeMatches || !propertiesMatches) {
                continue;
            }

            memTypeIdx = idx;
            break;
        }

        if (!memTypeIdx.has_value()) {
            throw EngineError("No memory type available for required allocation");
        }

        auto allocateInfo = vk::MemoryAllocateInfo()
                .setAllocationSize(requirements.size)
                .setMemoryTypeIndex(memTypeIdx.value());

        auto memory = deviceContext->getLogicalDevice().allocateMemory(allocateInfo);

        return {
                memory,
                [deviceContext](vk::DeviceMemory &instance) {
                    deviceContext->getLogicalDevice().free(instance);
                }
        };
    }

    DeviceMemory makeDeviceMemory(DeviceContext *deviceContext, const Image &image, bool local) {
        auto requirements = deviceContext->getLogicalDevice().getImageMemoryRequirements(image.getInstance());
        auto memory = makeDeviceMemory(deviceContext, requirements, local);

        deviceContext->getLogicalDevice().bindImageMemory(image.getInstance(), memory.getInstance(), 0);

        return memory;
    }

    DeviceMemory makeDeviceMemory(DeviceContext *deviceContext, const Buffer &buffer, bool local) {
        auto requirements = deviceContext->getLogicalDevice().getBufferMemoryRequirements(buffer.getInstance());
        auto memory = makeDeviceMemory(deviceContext, requirements, local);

        deviceContext->getLogicalDevice().bindBufferMemory(buffer.getInstance(), memory.getInstance(), 0);

        return memory;
    }
}
