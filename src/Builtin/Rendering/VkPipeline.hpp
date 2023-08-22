#ifndef PENROSE_BUILTIN_RENDERING_VK_PIPELINE_HPP
#define PENROSE_BUILTIN_RENDERING_VK_PIPELINE_HPP

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/Pipeline.hpp>

namespace Penrose {

    class DeviceContext;

    class VkPipeline : public Pipeline {
    public:
        VkPipeline(DeviceContext *deviceContext,
                   vk::DescriptorSetLayout descriptorSetLayout,
                   vk::PipelineLayout pipelineLayout,
                   vk::Pipeline pipeline);
        ~VkPipeline() override;

        [[nodiscard]] const vk::DescriptorSetLayout &getDescriptorSetLayout() const {
            return this->_descriptorSetLayout;
        }

        [[nodiscard]] const vk::PipelineLayout &getPipelineLayout() const { return this->_pipelineLayout; }

        [[nodiscard]] const vk::Pipeline &getPipeline() const { return this->_pipeline; }

    private:
        DeviceContext *_deviceContext;

        vk::DescriptorSetLayout _descriptorSetLayout;
        vk::PipelineLayout _pipelineLayout;
        vk::Pipeline _pipeline;
    };
}

#endif // PENROSE_BUILTIN_RENDERING_VK_PIPELINE_HPP
