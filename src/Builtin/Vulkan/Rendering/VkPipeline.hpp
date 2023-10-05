#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_VK_PIPELINE_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_VK_PIPELINE_HPP

#include <map>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/Pipeline.hpp>
#include <Penrose/Rendering/PipelineInfo.hpp>

namespace Penrose {

    class VkDescriptor;
    class VkDescriptorPoolManager;
    class VkLogicalDeviceContext;

    class VkPipeline : public Pipeline {
    public:
        VkPipeline(PipelineInfo pipelineInfo,
                   VkLogicalDeviceContext *logicalDeviceContext,
                   VkDescriptorPoolManager *descriptorPoolManager,
                   vk::DescriptorSetLayout descriptorSetLayout,
                   vk::PipelineLayout pipelineLayout,
                   vk::Pipeline pipeline);
        ~VkPipeline() override;

        [[nodiscard]] const PipelineInfo &getPipelineInfo() const override { return this->_pipelineInfo; }

        [[nodiscard]] const vk::DescriptorSetLayout &getDescriptorSetLayout() const {
            return this->_descriptorSetLayout;
        }

        [[nodiscard]] const vk::PipelineLayout &getPipelineLayout() const { return this->_pipelineLayout; }

        [[nodiscard]] const vk::Pipeline &getPipeline() const { return this->_pipeline; }

        [[nodiscard]] Descriptor *getDescriptorFor(const std::unordered_set<DescriptorBindingValue> &values) override;

    private:
        PipelineInfo _pipelineInfo;

        VkLogicalDeviceContext *_logicalDeviceContext;
        VkDescriptorPoolManager *_descriptorPoolManager;

        vk::DescriptorSetLayout _descriptorSetLayout;
        vk::PipelineLayout _pipelineLayout;
        vk::Pipeline _pipeline;

        std::map<std::size_t, VkDescriptor *> _descriptors;

        [[nodiscard]] VkDescriptor *createDescriptor();
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_VK_PIPELINE_HPP
