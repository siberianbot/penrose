#include <Penrose/Builtin/Rendering/ForwardSceneDrawRenderOperator.hpp>

#include <utility>

#include <glm/gtc/matrix_transform.hpp>

#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/Assets/ImageAsset.hpp>
#include <Penrose/Assets/MeshAsset.hpp>
#include <Penrose/Rendering/PipelineFactory.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

#include "src/Constants.hpp"
#include "src/Rendering/DeviceContext.hpp"
#include "src/Rendering/RenderData.hpp"
#include "src/Rendering/RenderListBuilder.hpp"

#include "src/Builtin/Rendering/VkBuffer.hpp"
#include "src/Builtin/Rendering/VkImage.hpp"
#include "src/Builtin/Rendering/VkPipeline.hpp"

namespace Penrose {

    ForwardSceneDrawRenderOperator::ForwardSceneDrawRenderOperator(AssetManager *assetManager,
                                                                   DeviceContext *deviceContext,
                                                                   RenderListBuilder *renderListBuilder,
                                                                   VkPipeline *pipeline,
                                                                   vk::Sampler sampler,
                                                                   std::string renderList)
            : _assetManager(assetManager),
              _deviceContext(deviceContext),
              _renderListBuilder(renderListBuilder),
              _pipeline(pipeline),
              _sampler(sampler),
              _renderList(std::move(renderList)) {
        //
    }

    ForwardSceneDrawRenderOperator::~ForwardSceneDrawRenderOperator() {
        this->_deviceContext->getLogicalDevice().destroy(this->_sampler);
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

        context.commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, this->_pipeline->getPipeline());

        for (const auto &[entity, drawable]: renderList->drawables) {
            auto maybeMesh = flatMap(drawable.meshAsset, [this](const std::string &asset) {
                return this->_assetManager->tryGetAsset<MeshAsset>(asset);
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

            auto vertexBuffer = dynamic_cast<VkBuffer *>(maybeMesh->get()->getVertexBuffer());
            auto indexBuffer = dynamic_cast<VkBuffer *>(maybeMesh->get()->getIndexBuffer());

            context.commandBuffer.pushConstants(this->_pipeline->getPipelineLayout(), vk::ShaderStageFlagBits::eVertex,
                                                0, sizeof(RenderData), &renderData);
            context.commandBuffer.bindVertexBuffers(0, vertexBuffer->getBuffer(), {0});
            context.commandBuffer.bindIndexBuffer(indexBuffer->getBuffer(), 0, vk::IndexType::eUint32);
            context.commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                                     this->_pipeline->getPipelineLayout(), 0,
                                                     {*maybeDescriptorSet}, {});

            context.commandBuffer.drawIndexed(indexBuffer->getCount(), 1, 0, 0, 0);
        }
    }

    std::optional<vk::DescriptorSet> ForwardSceneDrawRenderOperator::tryGetDescriptorSet(const Entity &entity,
                                                                                         const uint32_t &frameIdx,
                                                                                         const std::string &asset) {
        auto descriptorsIt = this->_descriptors.find(entity);
        if (descriptorsIt != this->_descriptors.end()) {
            return descriptorsIt->second.at(frameIdx);
        }

        auto maybeImage = this->_assetManager->tryGetAsset<ImageAsset>(asset);
        if (!maybeImage.has_value()) {
            return std::nullopt;
        }

        auto layouts = std::vector<vk::DescriptorSetLayout>(INFLIGHT_FRAME_COUNT);
        for (std::uint32_t idx = 0; idx < INFLIGHT_FRAME_COUNT; idx++) {
            layouts[idx] = this->_pipeline->getDescriptorSetLayout();
        }

        auto allocateInfo = vk::DescriptorSetAllocateInfo()
                .setSetLayouts(layouts)
                .setDescriptorPool(this->_deviceContext->getDescriptorPool());

        auto descriptorSets = this->_deviceContext->getLogicalDevice().allocateDescriptorSets(allocateInfo);
        auto writes = std::vector<vk::WriteDescriptorSet>(descriptorSets.size());

        auto image = dynamic_cast<VkImage *>(maybeImage->get()->getImage());
        auto imageInfo = vk::DescriptorImageInfo()
                .setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                .setImageView(image->getImageView())
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
              _pipelineFactory(resources->get<PipelineFactory>()),
              _renderListBuilder(resources->get<RenderListBuilder>()) {
        //
    }

    ParamsCollection ForwardSceneDrawRenderOperatorFactory::defaults() const {
        ParamsCollection params;
        params.setString(ForwardSceneDrawRenderOperator::PIPELINE_PARAM,
                         "Default");
        params.setString(ForwardSceneDrawRenderOperator::RENDER_LIST_PARAM,
                         "Default");

        return params;
    }

    RenderOperator *ForwardSceneDrawRenderOperatorFactory::create(const RenderOperatorFactory::Context &context) const {
        auto renderList = context.params.getString(ForwardSceneDrawRenderOperator::RENDER_LIST_PARAM);

        auto pipeline = this->_pipelineFactory->getPipeline("Default", context.subgraph, context.passIdx);

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
                                                  dynamic_cast<VkPipeline *>(pipeline),
                                                  sampler,
                                                  renderList);
    }
}
