#ifndef PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_PIPELINE_HPP
#define PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_PIPELINE_HPP

#include <vulkan/vulkan.hpp>

#include <Penrose/Rendering/Objects/Pipeline.hpp>
#include <Penrose/Rendering/Objects/PipelineInfo.hpp>

namespace Penrose {

    /**
     * \brief Pipeline implementation for Vulkan
     * \details Stores pipeline definition for VK and created layouts. Actual pipeline creates just-in-time.
     */
    class VkPipeline final: public Pipeline {
    public:
        VkPipeline(
            PipelineInfo &&pipelineInfo, std::vector<vk::VertexInputBindingDescription> &&vertexInputBindings,
            std::vector<vk::VertexInputAttributeDescription> &&vertexInputAttributes,
            vk::PipelineInputAssemblyStateCreateInfo &&inputAssemblyState,
            // TODO: vk::PipelineTessellationStateCreateInfo &&tessellationState,
            vk::PipelineViewportStateCreateInfo &&viewportState,
            vk::PipelineRasterizationStateCreateInfo &&rasterizationState,
            vk::PipelineMultisampleStateCreateInfo &&multisampleState,
            vk::PipelineDepthStencilStateCreateInfo &&depthStencilState,
            std::vector<vk::PipelineColorBlendAttachmentState> &&colorBlendAttachments,
            vk::PipelineColorBlendStateCreateInfo &&colorBlendState, std::vector<vk::DynamicState> &&dynamicStates,
            vk::UniqueDescriptorSetLayout &&descriptorSetLayout, vk::UniquePipelineLayout &&pipelineLayout
        )
            : _pipelineInfo(std::forward<decltype(pipelineInfo)>(pipelineInfo)),
              _vertexInputBindings(std::forward<decltype(vertexInputBindings)>(vertexInputBindings)),
              _vertexInputAttributes(std::forward<decltype(vertexInputAttributes)>(vertexInputAttributes)),
              _inputAssemblyState(std::forward<decltype(inputAssemblyState)>(inputAssemblyState)),
              // TODO:  _tessellationState(std::forward<decltype(tessellationState)>(tessellationState)),
              _viewportState(std::forward<decltype(viewportState)>(viewportState)),
              _rasterizationState(std::forward<decltype(rasterizationState)>(rasterizationState)),
              _multisampleState(std::forward<decltype(multisampleState)>(multisampleState)),
              _depthStencilState(std::forward<decltype(depthStencilState)>(depthStencilState)),
              _colorBlendAttachments(std::forward<decltype(colorBlendAttachments)>(colorBlendAttachments)),
              _colorBlendState(std::forward<decltype(colorBlendState)>(colorBlendState)),
              _dynamicStates(std::forward<decltype(dynamicStates)>(dynamicStates)),
              _descriptorSetLayout(std::forward<decltype(descriptorSetLayout)>(descriptorSetLayout)),
              _pipelineLayout(std::forward<decltype(pipelineLayout)>(pipelineLayout)) {
            //
        }

        ~VkPipeline() override = default;

        [[nodiscard]] const PipelineInfo &getPipelineInfo() const override { return this->_pipelineInfo; }

        [[nodiscard]] const std::vector<vk::VertexInputBindingDescription> &getVertexInputBindings() const {
            return this->_vertexInputBindings;
        }

        [[nodiscard]] const std::vector<vk::VertexInputAttributeDescription> &getVertexInputAttributes() const {
            return this->_vertexInputAttributes;
        }

        [[nodiscard]] const vk::PipelineInputAssemblyStateCreateInfo &getInputAssemblyState() const {
            return this->_inputAssemblyState;
        }

        // TODO:
        // [[nodiscard]] const vk::PipelineTessellationStateCreateInfo &getTessellationState() const {
        //     return this->_tessellationState;
        // }

        [[nodiscard]] const vk::PipelineViewportStateCreateInfo &getViewportState() const {
            return this->_viewportState;
        }

        [[nodiscard]] const vk::PipelineRasterizationStateCreateInfo &getRasterizationState() const {
            return this->_rasterizationState;
        }

        [[nodiscard]] const vk::PipelineMultisampleStateCreateInfo &getMultisampleState() const {
            return this->_multisampleState;
        }

        [[nodiscard]] const vk::PipelineDepthStencilStateCreateInfo &getDepthStencilState() const {
            return this->_depthStencilState;
        }

        [[nodiscard]] const std::vector<vk::PipelineColorBlendAttachmentState> &getColorBlendAttachments() const {
            return this->_colorBlendAttachments;
        }

        [[nodiscard]] const vk::PipelineColorBlendStateCreateInfo &getColorBlendState() const {
            return this->_colorBlendState;
        }

        [[nodiscard]] const std::vector<vk::DynamicState> &getDynamicStates() const { return this->_dynamicStates; }

        [[nodiscard]] const vk::DescriptorSetLayout &getDescriptorSetLayoutHandle() const {
            return this->_descriptorSetLayout.get();
        }

        [[nodiscard]] const vk::PipelineLayout &getPipelineLayoutHandle() const { return this->_pipelineLayout.get(); }

    private:
        PipelineInfo _pipelineInfo;

        std::vector<vk::VertexInputBindingDescription> _vertexInputBindings;
        std::vector<vk::VertexInputAttributeDescription> _vertexInputAttributes;
        vk::PipelineInputAssemblyStateCreateInfo _inputAssemblyState;
        // TODO: vk::PipelineTessellationStateCreateInfo _tessellationState;
        vk::PipelineViewportStateCreateInfo _viewportState;
        vk::PipelineRasterizationStateCreateInfo _rasterizationState;
        vk::PipelineMultisampleStateCreateInfo _multisampleState;
        vk::PipelineDepthStencilStateCreateInfo _depthStencilState;
        std::vector<vk::PipelineColorBlendAttachmentState> _colorBlendAttachments;
        vk::PipelineColorBlendStateCreateInfo _colorBlendState;
        std::vector<vk::DynamicState> _dynamicStates;

        vk::UniqueDescriptorSetLayout _descriptorSetLayout;
        vk::UniquePipelineLayout _pipelineLayout;
    };
}

#endif // PENROSE_BUILTIN_VULKAN_RENDERING_OBJECTS_VK_PIPELINE_HPP
