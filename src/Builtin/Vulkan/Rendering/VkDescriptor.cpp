#include "VkDescriptor.hpp"

#include <memory>
#include <vector>

#include <Penrose/Common/EngineError.hpp>

#include "src/Builtin/Vulkan/Rendering/VkBuffer.hpp"
#include "src/Builtin/Vulkan/Rendering/VkDescriptorPoolManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkImage.hpp"
#include "src/Builtin/Vulkan/Rendering/VkPipeline.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSampler.hpp"

namespace Penrose {

    VkDescriptor::VkDescriptor(VkDescriptorPoolManager *descriptorPoolManager,
                               VkLogicalDeviceContext *logicalDeviceContext,
                               DescriptorSets descriptorSets, VkPipeline *pipeline)
            : _descriptorPoolManager(descriptorPoolManager),
              _logicalDeviceContext(logicalDeviceContext),
              _descriptorSets(descriptorSets),
              _pipeline(pipeline) {
        //
    }

    VkDescriptor::~VkDescriptor() {
        this->_descriptorPoolManager->destroyDescriptorSets(this->_descriptorSets);
    }

    void VkDescriptor::updateBindingValues(const std::unordered_set<DescriptorBindingValue> &values) {

        if (this->_bindingValues == values) {
            return;
        }

        std::vector<std::unique_ptr<std::vector<vk::DescriptorBufferInfo>>> bufferData;
        std::vector<std::unique_ptr<std::vector<vk::DescriptorImageInfo>>> samplerData;
        std::vector<vk::WriteDescriptorSet> writes;

        for (const auto &descriptorSet: this->_descriptorSets) {
            for (const auto &value: values) {
                auto bindingIdx = value.getBindingIdx();
                auto binding = this->_pipeline->getPipelineInfo().getLayout().getBindings().at(bindingIdx);

                switch (binding.getType()) {
                    case PipelineLayoutBindingType::Sampler: {
                        auto datum = samplerData.emplace_back(createSamplerData(value)).get();

                        writes.emplace_back(
                                vk::WriteDescriptorSet()
                                        .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
                                        .setDescriptorCount(datum->size())
                                        .setPImageInfo(datum->data())
                                        .setDstBinding(bindingIdx)
                                        .setDstSet(descriptorSet)
                        );

                        break;
                    }

                    case PipelineLayoutBindingType::UniformBuffer: {
                        auto datum = bufferData.emplace_back(createUniformBufferData(value)).get();

                        writes.emplace_back(
                                vk::WriteDescriptorSet()
                                        .setDescriptorType(vk::DescriptorType::eUniformBuffer)
                                        .setDescriptorCount(datum->size())
                                        .setPBufferInfo(datum->data())
                                        .setDstBinding(bindingIdx)
                                        .setDstSet(descriptorSet)
                        );

                        break;
                    }

                    case PipelineLayoutBindingType::InputAttachment:
                        // skip
                        break;

                    default:
                        throw EngineError("Binding type is not supported");
                }
            }
        }

        this->_logicalDeviceContext->getHandle().updateDescriptorSets(writes, {});

        this->_bindingValues = values;
    }

    std::vector<vk::DescriptorBufferInfo> *VkDescriptor::createUniformBufferData(const DescriptorBindingValue &value) {
        auto data = new std::vector<vk::DescriptorBufferInfo>(value.getBuffers().size());
        std::transform(value.getBuffers().begin(), value.getBuffers().end(), data->begin(),
                       createBufferInfo);

        return data;
    }

    std::vector<vk::DescriptorImageInfo> *VkDescriptor::createSamplerData(const DescriptorBindingValue &value) {
        if (value.getImages().size() != value.getSamplers().size()) {
            throw EngineError("Binding {} requires equal count of images and samplers", value.getBindingIdx());
        }

        auto size = value.getImages().size();
        auto data = new std::vector<vk::DescriptorImageInfo>(size);

        for (std::uint32_t idx = 0; idx < size; ++idx) {
            data->at(idx) = createImageInfo(value.getImages().at(idx),
                                            value.getSamplers().at(idx));
        }

        return data;
    }

    vk::DescriptorBufferInfo VkDescriptor::createBufferInfo(Buffer *buffer) {
        auto vkBuffer = dynamic_cast<VkBuffer *>(buffer);

        return {
                vkBuffer->getBuffer(),
                0,
                buffer->getSize()
        };
    }

    vk::DescriptorImageInfo VkDescriptor::createImageInfo(Image *image, Sampler *sampler) {
        auto vkImage = dynamic_cast<VkImage *>(image);
        auto vkSampler = dynamic_cast<VkSampler *>(sampler);

        return {
                vkSampler->getSampler(),
                vkImage->getImageView(),
                vk::ImageLayout::eShaderReadOnlyOptimal
        };
    }
}
