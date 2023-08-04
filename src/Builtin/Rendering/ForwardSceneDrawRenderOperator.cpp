#include <Penrose/Builtin/Rendering/ForwardSceneDrawRenderOperator.hpp>

#include <utility>

#include <glm/gtc/matrix_transform.hpp>

#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/Common/Vertex.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

#include "src/Constants.hpp"
#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/RenderData.hpp"
#include "src/Rendering/RenderListBuilder.hpp"

#include "src/Builtin/Assets/VkImageAsset.hpp"
#include "src/Builtin/Assets/VkMeshAsset.hpp"
#include "src/Builtin/Assets/VkShaderAsset.hpp"

namespace Penrose {

    ForwardSceneDrawRenderOperator::ForwardSceneDrawRenderOperator(AssetManager *assetManager,
                                                                   DeviceContext *deviceContext,
                                                                   RenderListBuilder *renderListBuilder,
                                                                   vk::DescriptorSetLayout descriptorSetLayout,
                                                                   vk::PipelineLayout pipelineLayout,
                                                                   vk::Pipeline pipeline,
                                                                   vk::Sampler sampler,
                                                                   std::string renderList)
            : _assetManager(assetManager),
              _deviceContext(deviceContext),
              _renderListBuilder(renderListBuilder),
              _descriptorSetLayout(descriptorSetLayout),
              _pipelineLayout(pipelineLayout),
              _pipeline(pipeline),
              _sampler(sampler),
              _renderList(std::move(renderList)) {
        //
    }

    ForwardSceneDrawRenderOperator::~ForwardSceneDrawRenderOperator() {
        this->_deviceContext->getLogicalDevice().destroy(this->_sampler);
        this->_deviceContext->getLogicalDevice().destroy(this->_pipeline);
        this->_deviceContext->getLogicalDevice().destroy(this->_pipelineLayout);
        this->_deviceContext->getLogicalDevice().destroy(this->_descriptorSetLayout);
    }

    void ForwardSceneDrawRenderOperator::execute(const RenderOperator::Context &context) {
        auto renderList = this->_renderListBuilder->tryBuildRenderList(this->_renderList);

        if (!renderList.has_value()) {
            return;
        }

        auto projection = Penrose::ForwardSceneDrawRenderOperator::getProjection(context, &renderList->view);
        projection[1][1] *= -1;

        auto viewport = vk::Viewport()
                .setX(0)
                .setY(0)
                .setWidth(static_cast<float>(context.renderArea.extent.width))
                .setHeight(static_cast<float>(context.renderArea.extent.height))
                .setMinDepth(0)
                .setMaxDepth(1);

        context.commandBuffer.setViewport(0, viewport);
        context.commandBuffer.setScissor(0, context.renderArea);

        context.commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, this->_pipeline);

        for (const auto &[entity, drawable]: renderList->drawables) {
            auto maybeMesh = flatMap(drawable.meshAsset, [this](const std::string &asset) {
                return this->_assetManager->tryGetAsset<VkMeshAsset>(asset);
            });

            if (!maybeMesh.has_value()) {
                continue;
            }

            auto maybeDescriptorSet = flatMap(drawable.albedoTextureAsset,
                                              [this, &entity, &context](const std::string &asset) {
                                                  return this->tryGetDescriptorSet(entity, context.frameIdx, asset);
                                              });

            if (!maybeDescriptorSet.has_value()) {
                continue;
            }

            auto renderData = RenderData{
                    .matrix = projection * renderList->view.view * drawable.model,
                    .model = drawable.model,
                    .modelRot = drawable.modelRot
            };

            context.commandBuffer.pushConstants(this->_pipelineLayout, vk::ShaderStageFlagBits::eVertex,
                                                0, sizeof(RenderData), &renderData);
            context.commandBuffer.bindVertexBuffers(0, maybeMesh->get()->getVertexBuffer(), {0});
            context.commandBuffer.bindIndexBuffer(maybeMesh->get()->getIndexBuffer(), 0, vk::IndexType::eUint32);
            context.commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                                     this->_pipelineLayout, 0, {*maybeDescriptorSet}, {});

            context.commandBuffer.drawIndexed(maybeMesh->get()->getIndexCount(), 1, 0, 0, 0);
        }
    }

    std::optional<vk::DescriptorSet> ForwardSceneDrawRenderOperator::tryGetDescriptorSet(const Entity &entity,
                                                                                         const uint32_t &frameIdx,
                                                                                         const std::string &asset) {
        auto descriptorsIt = this->_descriptors.find(entity);
        if (descriptorsIt != this->_descriptors.end()) {
            return descriptorsIt->second.at(frameIdx);
        }

        auto maybeImage = this->_assetManager->tryGetAsset<VkImageAsset>(asset);
        if (!maybeImage.has_value()) {
            return std::nullopt;
        }

        auto layouts = std::vector<vk::DescriptorSetLayout>(INFLIGHT_FRAME_COUNT);
        for (std::uint32_t idx = 0; idx < INFLIGHT_FRAME_COUNT; idx++) {
            layouts[idx] = this->_descriptorSetLayout;
        }

        auto allocateInfo = vk::DescriptorSetAllocateInfo()
                .setSetLayouts(layouts)
                .setDescriptorPool(this->_deviceContext->getDescriptorPool());

        auto descriptorSets = this->_deviceContext->getLogicalDevice().allocateDescriptorSets(allocateInfo);
        auto writes = std::vector<vk::WriteDescriptorSet>(descriptorSets.size());

        auto imageInfo = vk::DescriptorImageInfo()
                .setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                .setImageView(maybeImage->get()->getImageView())
                .setSampler(this->_sampler);

        for (std::uint32_t idx = 0; idx < INFLIGHT_FRAME_COUNT; idx++) {
            writes[idx] = vk::WriteDescriptorSet()
                    .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
                    .setImageInfo(imageInfo)
                    .setDescriptorCount(1)
                    .setDstBinding(0)
                    .setDstSet(descriptorSets.at(idx));
        }

        this->_deviceContext->getLogicalDevice().updateDescriptorSets(writes, {});

        this->_descriptors[entity] = descriptorSets;

        return descriptorSets.at(frameIdx);
    }

    glm::mat4 ForwardSceneDrawRenderOperator::getProjection(const RenderOperator::Context &context, View *view) {
        if (auto perspective = std::get_if<Perspective>(&*view->projection)) {
            return glm::perspective(perspective->fov,
                                    static_cast<float>(context.renderArea.extent.width) /
                                    static_cast<float>(context.renderArea.extent.height),
                                    perspective->near,
                                    perspective->far);
        }

        return glm::mat4(1);
    }

    ForwardSceneDrawRenderOperatorFactory::ForwardSceneDrawRenderOperatorFactory(ResourceSet *resources)
            : _assetManager(resources->get<AssetManager>()),
              _deviceContext(resources->get<DeviceContext>()),
              _renderListBuilder(resources->get<RenderListBuilder>()) {
        //
    }

    ParamsCollection ForwardSceneDrawRenderOperatorFactory::defaults() const {
        ParamsCollection params;
        params.setString(ForwardSceneDrawRenderOperator::RENDER_LIST_PARAM,
                         "Default");
        params.setString(ForwardSceneDrawRenderOperator::VERTEX_SHADER_PARAM,
                         "shaders/default-forward-rendering.vert.spv");
        params.setString(ForwardSceneDrawRenderOperator::FRAGMENT_SHADER_PARAM,
                         "shaders/default-forward-rendering.frag.spv");

        return params;
    }

    RenderOperator *ForwardSceneDrawRenderOperatorFactory::create(const RenderOperatorFactory::Context &context) const {
        auto vertexShader = context.params.getString(ForwardSceneDrawRenderOperator::VERTEX_SHADER_PARAM);
        auto fragmentShader = context.params.getString(ForwardSceneDrawRenderOperator::FRAGMENT_SHADER_PARAM);
        auto renderList = context.params.getString(ForwardSceneDrawRenderOperator::RENDER_LIST_PARAM);

        auto vertexShaderAsset = this->_assetManager->getAsset<VkShaderAsset>(vertexShader);
        auto fragmentShaderAsset = this->_assetManager->getAsset<VkShaderAsset>(fragmentShader);

        auto stages = {
                vk::PipelineShaderStageCreateInfo()
                        .setStage(vk::ShaderStageFlagBits::eVertex)
                        .setModule(vertexShaderAsset->getShaderModule())
                        .setPName("main"),
                vk::PipelineShaderStageCreateInfo()
                        .setStage(vk::ShaderStageFlagBits::eFragment)
                        .setModule(fragmentShaderAsset->getShaderModule())
                        .setPName("main")
        };

        auto pushConstants = {
                vk::PushConstantRange()
                        .setStageFlags(vk::ShaderStageFlagBits::eVertex)
                        .setOffset(0)
                        .setSize(sizeof(RenderData))
        };

        auto descriptorSetBindings = {
                vk::DescriptorSetLayoutBinding()
                        .setBinding(0)
                        .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
                        .setDescriptorCount(1)
                        .setStageFlags(vk::ShaderStageFlagBits::eFragment)
        };

        auto descriptorSetLayoutCreateInfo = vk::DescriptorSetLayoutCreateInfo()
                .setBindings(descriptorSetBindings);

        auto descriptorSetLayout = this->_deviceContext->getLogicalDevice()
                .createDescriptorSetLayout(descriptorSetLayoutCreateInfo);

        auto pipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo()
                .setPushConstantRanges(pushConstants)
                .setSetLayouts(descriptorSetLayout);

        auto pipelineLayout = this->_deviceContext->getLogicalDevice()
                .createPipelineLayout(pipelineLayoutCreateInfo);

        auto bindings = {
                vk::VertexInputBindingDescription(0, sizeof(Vertex), vk::VertexInputRate::eVertex)
        };

        auto attributes = {
                vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, pos)),
                vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, normal)),
                vk::VertexInputAttributeDescription(2, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color)),
                vk::VertexInputAttributeDescription(3, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, uv))
        };

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
                .setRenderPass(context.renderPass)
                .setSubpass(context.subpassIdx)
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

        auto [_, pipeline] = this->_deviceContext->getLogicalDevice()
                .createGraphicsPipeline(nullptr, pipelineCreateInfo);

        auto samplerCreateInfo = vk::SamplerCreateInfo()
                .setAddressModeU(vk::SamplerAddressMode::eRepeat)
                .setAddressModeV(vk::SamplerAddressMode::eRepeat)
                .setAddressModeW(vk::SamplerAddressMode::eRepeat)
                .setAnisotropyEnable(false)
                .setMaxAnisotropy(1)
                .setBorderColor(vk::BorderColor::eFloatOpaqueBlack)
                .setCompareEnable(false)
                .setMinFilter(vk::Filter::eLinear)
                .setMagFilter(vk::Filter::eLinear)
                .setMinLod(0)
                .setMaxLod(1)
                .setUnnormalizedCoordinates(false)
                .setMipmapMode(vk::SamplerMipmapMode::eLinear);

        auto sampler = this->_deviceContext->getLogicalDevice().createSampler(samplerCreateInfo);

        return new ForwardSceneDrawRenderOperator(this->_assetManager,
                                                  this->_deviceContext,
                                                  this->_renderListBuilder,
                                                  descriptorSetLayout,
                                                  pipelineLayout,
                                                  pipeline,
                                                  sampler,
                                                  renderList);
    }
}
