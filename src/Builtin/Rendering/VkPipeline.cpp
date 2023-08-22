#include "VkPipeline.hpp"

#include "src/Rendering/DeviceContext.hpp"

namespace Penrose {

    VkPipeline::VkPipeline(DeviceContext *deviceContext,
                           vk::DescriptorSetLayout descriptorSetLayout,
                           vk::PipelineLayout pipelineLayout,
                           vk::Pipeline pipeline)
            : _deviceContext(deviceContext),
              _descriptorSetLayout(descriptorSetLayout),
              _pipelineLayout(pipelineLayout),
              _pipeline(pipeline) {
        //
    }

    VkPipeline::~VkPipeline() {
        this->_deviceContext->getLogicalDevice().destroy(this->_pipeline);
        this->_deviceContext->getLogicalDevice().destroy(this->_pipelineLayout);
        this->_deviceContext->getLogicalDevice().destroy(this->_descriptorSetLayout);
    }
}
