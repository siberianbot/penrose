#include <Penrose/Builtin/Rendering/ForwardSceneDrawRenderOperator.hpp>

#include <utility>

#include <glm/gtc/matrix_transform.hpp>

#include <Penrose/Rendering/RenderContext.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include "src/Assets/AssetManager.hpp"
#include "src/Common/Vertex.hpp"
#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/RenderData.hpp"

namespace Penrose {

    MeshAsset *ForwardSceneDrawRenderOperator::getMesh(const AssetId &asset) {
        auto meshIt = this->_meshes.find(asset);

        if (meshIt != this->_meshes.end()) {
            return &meshIt->second;
        }

        auto [it, _] = this->_meshes.emplace(asset, this->_assetManager->loadMesh(asset));

        return &it->second;
    }

    ForwardSceneDrawRenderOperator::ForwardSceneDrawRenderOperator(AssetManager *assetManager,
                                                                   RenderContext *renderContext,
                                                                   PipelineLayout &&pipelineLayout,
                                                                   Pipeline &&pipeline,
                                                                   std::string &&renderList)
            : _assetManager(assetManager),
              _renderContext(renderContext),
              _pipelineLayout(std::move(pipelineLayout)),
              _pipeline(std::move(pipeline)),
              _renderList(std::move(renderList)) {
        //
    }

    void ForwardSceneDrawRenderOperator::execute(const RenderOperatorExecutionContext &context) {
        auto lock = this->_renderContext->acquireContextLock();
        auto renderList = this->_renderContext->tryGetRenderList(this->_renderList);

        if (!renderList.has_value()) {
            return;
        }

        auto projection = glm::mat4(1);
        if (renderList->projection.has_value()) {
            if (auto perspective = std::get_if<Perspective>(&renderList->projection.value())) {
                projection = glm::perspective(perspective->fov,
                                              static_cast<float>(context.renderArea.extent.width) /
                                              static_cast<float>(context.renderArea.extent.height),
                                              renderList->near,
                                              renderList->far);

                projection[1][1] *= -1;
            }
        }

        auto viewport = vk::Viewport()
                .setX(0)
                .setY(0)
                .setWidth(static_cast<float>(context.renderArea.extent.width))
                .setHeight(static_cast<float>(context.renderArea.extent.height))
                .setMinDepth(0)
                .setMaxDepth(1);

        context.commandBuffer.setViewport(0, viewport);
        context.commandBuffer.setScissor(0, context.renderArea);

        context.commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, this->_pipeline.getInstance());

        for (const auto &[_, item]: renderList->items) {
            auto mesh = this->getMesh(item.mesh);

            auto renderData = RenderData{
                    .matrix = projection * renderList->view * item.model,
                    .model = item.model,
                    .modelRot = item.modelRot
            };

            context.commandBuffer.pushConstants(this->_pipelineLayout.getInstance(), vk::ShaderStageFlagBits::eVertex,
                                                0, sizeof(RenderData), &renderData);
            context.commandBuffer.bindVertexBuffers(0, mesh->vertexBuffer.getInstance(), {0});
            context.commandBuffer.bindIndexBuffer(mesh->indexBuffer.getInstance(), 0, vk::IndexType::eUint32);

            context.commandBuffer.drawIndexed(mesh->indexCount, 1, 0, 0, 0);
        }
    }

    ParamsCollection ForwardSceneDrawRenderOperator::defaults() {
        ParamsCollection params;
        params.setString(RENDER_LIST_PARAM, "Default");
        params.setString(VERTEX_SHADER_PARAM, "shaders/default-forward-rendering.vert.spv");
        params.setString(FRAGMENT_SHADER_PARAM, "shaders/default-forward-rendering.frag.spv");

        return params;
    }

    std::unique_ptr<RenderOperator> ForwardSceneDrawRenderOperator::create(const RenderOperatorCreateContext &context) {
        auto assetManager = context.resources->get<AssetManager>();
        auto deviceContext = context.resources->get<DeviceContext>();

        auto vertexShader = assetManager->loadShader(context.params.getString(VERTEX_SHADER_PARAM));
        auto fragmentShader = assetManager->loadShader(context.params.getString(FRAGMENT_SHADER_PARAM));

        auto stages = {
                vk::PipelineShaderStageCreateInfo()
                        .setStage(vk::ShaderStageFlagBits::eVertex)
                        .setModule(vertexShader.shaderModule.getInstance())
                        .setPName("main"),
                vk::PipelineShaderStageCreateInfo()
                        .setStage(vk::ShaderStageFlagBits::eFragment)
                        .setModule(fragmentShader.shaderModule.getInstance())
                        .setPName("main")
        };

        auto pushConstants = {
                vk::PushConstantRange()
                        .setStageFlags(vk::ShaderStageFlagBits::eVertex)
                        .setOffset(0)
                        .setSize(sizeof(RenderData))
        };

        auto pipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo()
                .setPushConstantRanges(pushConstants);

        auto pipelineLayout = makePipelineLayout(deviceContext, pipelineLayoutCreateInfo);

        auto bindings = {
                vk::VertexInputBindingDescription(0, sizeof(Vertex), vk::VertexInputRate::eVertex)
        };

        auto attributes = {
                vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32Sfloat),
                vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32Sfloat),
                vk::VertexInputAttributeDescription(2, 0, vk::Format::eR32G32B32Sfloat),
                vk::VertexInputAttributeDescription(3, 0, vk::Format::eR32G32Sfloat)
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
                .setLayout(pipelineLayout.getInstance())
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

        auto pipeline = makeGraphicsPipeline(deviceContext, pipelineCreateInfo);

        return std::make_unique<ForwardSceneDrawRenderOperator>(context.resources->get<AssetManager>(),
                                                                context.resources->get<RenderContext>(),
                                                                std::move(pipelineLayout),
                                                                std::move(pipeline),
                                                                context.params.getString(RENDER_LIST_PARAM));
    }
}
