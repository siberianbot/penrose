#ifndef PENROSE_BUILTIN_RENDERING_VK_PIPELINE_HPP
#define PENROSE_BUILTIN_RENDERING_VK_PIPELINE_HPP

#include <map>

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/Pipeline.hpp>
#include <Penrose/Rendering/PipelineInfo.hpp>

namespace Penrose {

    class DeviceContext;
    class VkDescriptor;

    class VkPipeline : public Pipeline {
    public:
        VkPipeline(PipelineInfo pipelineInfo,
                   DeviceContext *deviceContext,
                   vk::DescriptorSetLayout descriptorSetLayout,
                   vk::PipelineLayout pipelineLayout,
                   vk::Pipeline pipeline);
        ~VkPipeline() override;

        [[nodiscard]] const vk::DescriptorSetLayout &getDescriptorSetLayout() const {
            return this->_descriptorSetLayout;
        }

        [[nodiscard]] const vk::PipelineLayout &getPipelineLayout() const { return this->_pipelineLayout; }

        [[nodiscard]] const vk::Pipeline &getPipeline() const { return this->_pipeline; }

        [[nodiscard]] Descriptor *getDescriptorFor(const Entity &entity,
                                                   const std::set<DescriptorBindingValue> &values) override;

    private:
        PipelineInfo _pipelineInfo;

        DeviceContext *_deviceContext;

        vk::DescriptorSetLayout _descriptorSetLayout;
        vk::PipelineLayout _pipelineLayout;
        vk::Pipeline _pipeline;

        std::map<Entity, VkDescriptor *> _descriptors;

        [[nodiscard]] VkDescriptor *createDescriptor() const;
    };
}

#endif // PENROSE_BUILTIN_RENDERING_VK_PIPELINE_HPP
