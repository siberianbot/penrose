#include "VkPipeline.hpp"

#include <fmt/core.h>

#include <Penrose/Common/EngineError.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

#include "src/Builtin/Vulkan/Constants.hpp"

#include "src/Builtin/Vulkan/Rendering/VkBuffer.hpp"
#include "src/Builtin/Vulkan/Rendering/VkDescriptor.hpp"
#include "src/Builtin/Vulkan/Rendering/VkDescriptorPoolManager.hpp"
#include "src/Builtin/Vulkan/Rendering/VkImage.hpp"
#include "src/Builtin/Vulkan/Rendering/VkLogicalDeviceContext.hpp"
#include "src/Builtin/Vulkan/Rendering/VkSampler.hpp"

namespace Penrose {

    VkPipeline::VkPipeline(PipelineInfo pipelineInfo,
                           VkLogicalDeviceContext *logicalDeviceContext,
                           VkDescriptorPoolManager *descriptorPoolManager,
                           vk::DescriptorSetLayout descriptorSetLayout,
                           vk::PipelineLayout pipelineLayout,
                           vk::Pipeline pipeline)
            : _pipelineInfo(std::move(pipelineInfo)),
              _logicalDeviceContext(logicalDeviceContext),
              _descriptorPoolManager(descriptorPoolManager),
              _descriptorSetLayout(descriptorSetLayout),
              _pipelineLayout(pipelineLayout),
              _pipeline(pipeline) {
        //
    }

    VkPipeline::~VkPipeline() {
        this->_logicalDeviceContext->getHandle().destroy(this->_pipeline);
        this->_logicalDeviceContext->getHandle().destroy(this->_pipelineLayout);
        this->_logicalDeviceContext->getHandle().destroy(this->_descriptorSetLayout);
    }

    Descriptor *VkPipeline::getDescriptorFor(const Entity &entity,
                                             const std::unordered_set<DescriptorBindingValue> &values) {
        auto descriptorIt = this->_descriptors.find(entity);

        if (descriptorIt != this->_descriptors.end() && descriptorIt->second->getBindingValues() == values) {
            return descriptorIt->second;
        }

        auto descriptor = descriptorIt != this->_descriptors.end()
                          ? descriptorIt->second
                          : this->createDescriptor();

        const auto &descriptorSets = descriptor->getDescriptorSets();

        std::vector<vk::DescriptorBufferInfo *> bufferInfos;
        std::vector<vk::DescriptorImageInfo *> imageInfos;
        std::vector<vk::WriteDescriptorSet> writes;

        for (const auto &descriptorSet: descriptorSets) {
            for (const auto &value: values) {
                auto bindingIdx = value.getBindingIdx();
                auto binding = this->_pipelineInfo.getLayout().getBindings().at(bindingIdx);

                switch (binding.getType()) {
                    case PipelineLayoutBindingType::Sampler: {
                        auto image = dynamic_cast<VkImage *>(orElseThrow(
                                value.getImage(),
                                EngineError(fmt::format("Binding {} requires image", bindingIdx))));

                        auto sampler = dynamic_cast<VkSampler *>(orElseThrow(
                                value.getSampler(),
                                EngineError(fmt::format("Binding {} requires sampler", bindingIdx))));

                        auto imageInfo = new vk::DescriptorImageInfo(
                                sampler->getSampler(),
                                image->getImageView(),
                                vk::ImageLayout::eShaderReadOnlyOptimal
                        );

                        writes.emplace_back(
                                vk::WriteDescriptorSet()
                                        .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
                                        .setDescriptorCount(1)
                                        .setPImageInfo(imageInfo)
                                        .setDstBinding(bindingIdx)
                                        .setDstSet(descriptorSet)
                        );

                        imageInfos.push_back(imageInfo);
                        break;
                    }

                    case PipelineLayoutBindingType::UniformBuffer: {
                        auto buffer = dynamic_cast<VkBuffer *>(orElseThrow(
                                value.getBuffer(),
                                EngineError(fmt::format("Binding {} requires buffer", bindingIdx))));

                        auto bufferInfo = new vk::DescriptorBufferInfo(
                                buffer->getBuffer(),
                                0,
                                buffer->getSize()
                        );

                        writes.emplace_back(
                                vk::WriteDescriptorSet()
                                        .setDescriptorType(vk::DescriptorType::eUniformBuffer)
                                        .setDescriptorCount(1)
                                        .setPBufferInfo(bufferInfo)
                                        .setDstBinding(bindingIdx)
                                        .setDstSet(descriptorSet)
                        );

                        bufferInfos.push_back(bufferInfo);
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

        for (auto &bufferInfo: bufferInfos) {
            delete bufferInfo;
        }

        for (auto &imageInfo: imageInfos) {
            delete imageInfo;
        }

        descriptor->setBindingValues(values);

        this->_descriptors.emplace(entity, descriptor);

        return descriptor;
    }

    VkDescriptor *VkPipeline::createDescriptor() {
        auto descriptorSets = this->_descriptorPoolManager->createDescriptorSets(this->_descriptorSetLayout);

        return new VkDescriptor(this->_descriptorPoolManager, descriptorSets);
    }
}
