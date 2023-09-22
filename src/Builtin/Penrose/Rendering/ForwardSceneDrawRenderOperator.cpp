#include <Penrose/Builtin/Penrose/Rendering/ForwardSceneDrawRenderOperator.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/Assets/ImageAsset.hpp>
#include <Penrose/Assets/MeshAsset.hpp>
#include <Penrose/Rendering/CommandRecording.hpp>
#include <Penrose/Rendering/Pipeline.hpp>
#include <Penrose/Rendering/PipelineFactory.hpp>
#include <Penrose/Rendering/RenderSubgraph.hpp>
#include <Penrose/Rendering/SamplerFactory.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

#include "src/Rendering/RenderData.hpp"
#include "src/Rendering/RenderListBuilder.hpp"

namespace Penrose {

    constexpr static const SamplerInfo DEFAULT_SAMPLER_INFO = SamplerInfo()
            .setAddressMode(SamplerAddressMode::Repeat)
            .setMinFilter(SamplerFilter::Linear)
            .setMagFilter(SamplerFilter::Linear)
            .setBorderColor(SamplerBorderColor::Black);

    ForwardSceneDrawRenderOperator::ForwardSceneDrawRenderOperator(ResourceSet *resources)
            : _assetManager(resources->get<AssetManager>()),
              _pipelineFactory(resources->get<PipelineFactory>()),
              _renderListBuilder(resources->get<RenderListBuilder>()),
              _samplerFactory(resources->get<SamplerFactory>()) {
        //
    }

    void ForwardSceneDrawRenderOperator::init() {
        this->_sampler = std::unique_ptr<Sampler>(this->_samplerFactory->makeSampler(DEFAULT_SAMPLER_INFO));
    }

    void ForwardSceneDrawRenderOperator::destroy() {
        this->_sampler = std::nullopt;
    }

    ParamsCollection ForwardSceneDrawRenderOperator::getDefaults() const {
        ParamsCollection params;

        params.setString(PARAM_PIPELINE, "Default");
        params.setString(PARAM_RENDER_LIST, "Default");

        return params;
    }

    void ForwardSceneDrawRenderOperator::execute(CommandRecording *commandRecording,
                                                 const RenderOperator::Context &context) {

        auto pipelineName = context.param.getString(PARAM_PIPELINE);
        auto renderListName = context.param.getString(PARAM_RENDER_LIST);

        auto renderList = this->_renderListBuilder->tryBuildRenderList(renderListName);

        if (!renderList.has_value()) {
            return;
        }

        auto &[w, h] = context.renderArea;

        auto projection = ForwardSceneDrawRenderOperator::getProjection(context, &renderList->view);
        projection[1][1] *= -1;

        commandRecording->setViewport(FloatRect(0, 0, w, h));
        commandRecording->setScissor(IntRect(0, 0, w, h));

        auto pipeline = this->_pipelineFactory->getPipeline(pipelineName, context.subgraph, context.subgraphPassIdx);
        commandRecording->bindGraphicsPipeline(pipeline);

        for (const auto &[entity, drawable]: renderList->drawables) {
            auto maybeMesh = flatMap(drawable.meshAsset, [this](const std::string &asset) {
                return this->_assetManager->tryGetAsset<MeshAsset>(asset);
            });

            if (!maybeMesh.has_value()) {
                continue;
            }

            auto maybeImage = flatMap(drawable.albedoTextureAsset,
                                      [this](const std::string &asset) {
                                          return this->_assetManager->tryGetAsset<ImageAsset>(asset);
                                      });
            if (!maybeImage.has_value()) {
                continue;
            }

            auto descriptor = pipeline->getDescriptorFor(entity, {
                    DescriptorBindingValue(0)
                            .setImage(maybeImage->get()->getImage())
                            .setSampler(this->_sampler->get())
            });

            commandRecording->bindDescriptor(pipeline, descriptor);

            auto renderData = RenderData{
                    .matrix = projection * renderList->view.view * drawable.model,
                    .model = drawable.model,
                    .modelRot = drawable.modelRot
            };

            commandRecording->bindPushConstants(pipeline, 0, &renderData);

            commandRecording->draw(maybeMesh->get()->getVertexBuffer(),
                                   maybeMesh->get()->getIndexBuffer());
        }
    }

    glm::mat4 ForwardSceneDrawRenderOperator::getProjection(const RenderOperator::Context &context, View *view) {
        auto &[renderAreaWidth, renderAreaHeight] = context.renderArea;

        if (auto perspective = std::get_if<Perspective>(&*view->projection)) {
            return glm::perspective(perspective->fov,
                                    static_cast<float>(renderAreaWidth) / static_cast<float>(renderAreaHeight),
                                    perspective->near,
                                    perspective->far);
        }

        return glm::mat4(1);
    }
}
