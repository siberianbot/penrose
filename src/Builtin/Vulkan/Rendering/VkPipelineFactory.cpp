#include "VkPipelineFactory.hpp"

#include <algorithm>

#include <fmt/core.h>

#include <Penrose/Assets/ShaderAsset.hpp>
#include <Penrose/Common/EngineError.hpp>

#include "src/Builtin/Vulkan/Rendering/VkPipeline.hpp"
#include "src/Builtin/Vulkan/Rendering/VkShader.hpp"
#include "src/Builtin/Vulkan/Rendering/VkUtils.hpp"

namespace Penrose {

    VkPipelineFactory::VkPipelineFactory(ResourceSet *resources)
            : _assetManager(resources->get<AssetManager>()),
              _descriptorPoolManager(resources->get<VkDescriptorPoolManager>()),
              _logicalDeviceContext(resources->get<VkLogicalDeviceContext>()) {
        //
    }

    void VkPipelineFactory::init() {
        auto createInfo = vk::PipelineCacheCreateInfo()
                .setPInitialData(nullptr)
                .setInitialDataSize(0);

        this->_pipelineCache = this->_logicalDeviceContext->getHandle().createPipelineCache(createInfo);
    }

    void VkPipelineFactory::destroy() {
        for (const auto &[_, pipeline]: this->_pipelines) {
            delete pipeline;
        }

        this->_pipelines.clear();

        this->_logicalDeviceContext->getHandle().destroy(this->_pipelineCache);
    }

    void VkPipelineFactory::addPipeline(const std::string &pipelineName, const PipelineInfo &pipelineInfo) {
        this->_pipelineInfos.emplace(pipelineName, pipelineInfo);
    }

    Pipeline *VkPipelineFactory::getPipeline(const std::string &pipelineName,
                                             const RenderSubgraph *subgraph,
                                             std::uint32_t passIdx) {
        PipelineKey key = std::make_tuple(pipelineName, subgraph, passIdx);

        auto it = this->_pipelines.find(key);

        if (it != this->_pipelines.end()) {
            return it->second;
        }

        auto pipeline = this->createPipeline(pipelineName, dynamic_cast<const VkRenderSubgraph *>(subgraph), passIdx);

        this->_pipelines.insert_or_assign(key, pipeline);

        return pipeline;
    }

    Pipeline *VkPipelineFactory::createPipeline(const std::string &pipelineName,
                                                const VkRenderSubgraph *subgraph,
                                                std::uint32_t passIdx) {
        auto it = this->_pipelineInfos.find(pipelineName);

        if (it == this->_pipelineInfos.end()) {
            throw EngineError(fmt::format("No such pipeline {}", pipelineName));
        }

        auto pipelineInfo = it->second;

        auto descriptorSetBindingsCount = pipelineInfo.getLayout().getBindings().size();
        auto descriptorSetBindings = std::vector<vk::DescriptorSetLayoutBinding>(descriptorSetBindingsCount);
        for (std::uint32_t bindingIdx = 0; bindingIdx < descriptorSetBindingsCount; bindingIdx++) {
            auto binding = pipelineInfo.getLayout()
                    .getBindings()
                    .at(bindingIdx);

            descriptorSetBindings[bindingIdx] = vk::DescriptorSetLayoutBinding()
                    .setBinding(bindingIdx)
                    .setDescriptorType(toVkDescriptorType(binding.getType()))
                    .setDescriptorCount(binding.getCount())
                    .setStageFlags(toVkShaderStageFlags(binding.getShaderStageType()));
        }

        auto descriptorSetLayoutCreateInfo = vk::DescriptorSetLayoutCreateInfo()
                .setBindings(descriptorSetBindings);

        auto descriptorSetLayout = this->_logicalDeviceContext->getHandle()
                .createDescriptorSetLayout(descriptorSetLayoutCreateInfo);

        auto pushConstants = std::vector<vk::PushConstantRange>(pipelineInfo.getLayout().getConstants().size());
        std::transform(pipelineInfo.getLayout().getConstants().begin(), pipelineInfo.getLayout().getConstants().end(),
                       pushConstants.begin(),
                       [](const PipelineLayoutConstant &constant) {
                           return vk::PushConstantRange()
                                   .setOffset(constant.getOffset())
                                   .setSize(constant.getSize())
                                   .setStageFlags(toVkShaderStageFlags(constant.getShaderStageType()));
                       });

        auto pipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo()
                .setPushConstantRanges(pushConstants)
                .setSetLayouts(descriptorSetLayout);

        auto pipelineLayout = this->_logicalDeviceContext->getHandle()
                .createPipelineLayout(pipelineLayoutCreateInfo);

        auto stages = std::vector<vk::PipelineShaderStageCreateInfo>(pipelineInfo.getStages().size());
        std::transform(
            pipelineInfo.getStages().begin(), pipelineInfo.getStages().end(), stages.begin(),
            [this](const PipelineShaderStage &stage) {
                auto asset = this->_assetManager->getAsset<ShaderAsset>(stage.getShaderAssetName());
                auto shader = asset->getShader().lock();

                return vk::PipelineShaderStageCreateInfo()
                    .setStage(toVkShaderStageFlagBits(stage.getType()))
                    .setModule(dynamic_cast<VkShader *>(shader.get())->getShaderModule())
                    .setPName("main");
            }
        );

        std::vector<vk::VertexInputBindingDescription> bindings;
        std::vector<vk::VertexInputAttributeDescription> attributes;
        std::uint32_t attributeIdx = 0;
        for (std::uint32_t bindingIdx = 0; bindingIdx < pipelineInfo.getBindings().size(); bindingIdx++) {
            auto binding = pipelineInfo.getBindings().at(bindingIdx);

            bindings.emplace_back(bindingIdx, binding.getStride(), toVkVertexInputRate(binding.getInputRate()));

            for (const auto &attribute: binding.getAttributes()) {
                auto vkFormat = toVkFormat(attribute.getFormat());

                switch (attribute.getFormat()) {
                    case PipelineBindingAttributeFormat::UInt:
                    case PipelineBindingAttributeFormat::Vec2:
                    case PipelineBindingAttributeFormat::Vec3:
                        attributes.emplace_back(attributeIdx++, bindingIdx, vkFormat, attribute.getOffset());

                        break;

                    case PipelineBindingAttributeFormat::Mat4: {
                        const std::uint32_t vecSize = 16;
                        const std::uint32_t matSize = 4 * vecSize;

                        auto baseOffset = attribute.getOffset();
                        auto targetOffset = baseOffset + matSize;

                        for (std::uint32_t offset = baseOffset; offset < targetOffset; offset += vecSize) {
                            attributes.emplace_back(attributeIdx++, bindingIdx, vkFormat, offset);
                        }

                        break;
                    }

                    default:
                        throw EngineError("Binding attribute format is not supported");

                }
            }
        }

        auto vertexInputState = vk::PipelineVertexInputStateCreateInfo()
                .setVertexBindingDescriptions(bindings)
                .setVertexAttributeDescriptions(attributes);

        auto inputAssemblyState = vk::PipelineInputAssemblyStateCreateInfo()
                .setTopology(vk::PrimitiveTopology::eTriangleList);

        auto viewportState = vk::PipelineViewportStateCreateInfo()
                .setScissorCount(1)
                .setViewportCount(1);

        auto depthStencilState = vk::PipelineDepthStencilStateCreateInfo()
                .setDepthTestEnable(true)
                .setDepthWriteEnable(true)
                .setDepthCompareOp(vk::CompareOp::eLessOrEqual)
                .setDepthBoundsTestEnable(false)
                .setStencilTestEnable(false)
                .setFront({})
                .setBack({})
                .setMinDepthBounds(0)
                .setMaxDepthBounds(0);

        auto rasterizationState = vk::PipelineRasterizationStateCreateInfo()
                .setDepthClampEnable(false)
                .setRasterizerDiscardEnable(false)
                .setPolygonMode(vk::PolygonMode::eFill)
                .setCullMode(vk::CullModeFlagBits::eBack)
                .setFrontFace(vk::FrontFace::eCounterClockwise)
                .setDepthBiasEnable(false)
                .setDepthBiasConstantFactor(0)
                .setDepthBiasClamp(0)
                .setDepthBiasSlopeFactor(0)
                .setLineWidth(1);

        auto colorBlendAttachments = {
                vk::PipelineColorBlendAttachmentState()
                        .setBlendEnable(false)
                        .setSrcColorBlendFactor({})
                        .setDstColorBlendFactor({})
                        .setColorBlendOp(vk::BlendOp::eAdd)
                        .setSrcAlphaBlendFactor({})
                        .setDstAlphaBlendFactor({})
                        .setAlphaBlendOp(vk::BlendOp::eAdd)
                        .setColorWriteMask(vk::ColorComponentFlagBits::eR |
                                           vk::ColorComponentFlagBits::eG |
                                           vk::ColorComponentFlagBits::eB |
                                           vk::ColorComponentFlagBits::eA)
        };

        auto colorBlendState = vk::PipelineColorBlendStateCreateInfo()
                .setLogicOpEnable(false)
                .setLogicOp(vk::LogicOp::eCopy)
                .setAttachments(colorBlendAttachments)
                .setBlendConstants({0, 0, 0, 0});

        auto dynamicStates = {
                vk::DynamicState::eViewport,
                vk::DynamicState::eScissor
        };

        auto dynamicState = vk::PipelineDynamicStateCreateInfo()
                .setDynamicStates(dynamicStates);

        auto multisampleState = vk::PipelineMultisampleStateCreateInfo()
                .setRasterizationSamples(vk::SampleCountFlagBits::e1)
                .setSampleShadingEnable(false)
                .setMinSampleShading(0)
                .setAlphaToCoverageEnable(false)
                .setAlphaToOneEnable(false);

        auto pipelineCreateInfo = vk::GraphicsPipelineCreateInfo()
                .setRenderPass(subgraph->getRenderPass())
                .setSubpass(passIdx)
                .setLayout(pipelineLayout)
                .setStages(stages)
                .setPVertexInputState(&vertexInputState)
                .setPInputAssemblyState(&inputAssemblyState)
                .setPViewportState(&viewportState)
                .setPRasterizationState(&rasterizationState)
                .setPDepthStencilState(&depthStencilState)
                .setPMultisampleState(&multisampleState)
                .setPTessellationState(nullptr)
                .setPColorBlendState(&colorBlendState)
                .setPDynamicState(&dynamicState);

        auto [_, pipeline] = this->_logicalDeviceContext->getHandle()
                .createGraphicsPipeline(this->_pipelineCache, pipelineCreateInfo);

        return new VkPipeline(pipelineInfo,
                              this->_logicalDeviceContext.get(),
                              this->_descriptorPoolManager.get(),
                              descriptorSetLayout,
                              pipelineLayout,
                              pipeline);
    }
}
