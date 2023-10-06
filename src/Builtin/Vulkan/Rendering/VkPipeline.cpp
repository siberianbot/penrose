#include "VkPipeline.hpp"

#include "src/Builtin/Vulkan/Rendering/VkDescriptor.hpp"
#include "src/Builtin/Vulkan/Rendering/VkDescriptorPoolManager.hpp"

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

    Descriptor *VkPipeline::allocateDescriptor() {
        auto descriptorSets = this->_descriptorPoolManager->createDescriptorSets(this->_descriptorSetLayout);

        return new VkDescriptor(this->_descriptorPoolManager, this->_logicalDeviceContext, descriptorSets, this);
    }
}
