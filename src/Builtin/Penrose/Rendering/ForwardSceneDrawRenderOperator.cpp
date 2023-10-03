#include <Penrose/Builtin/Penrose/Rendering/ForwardSceneDrawRenderOperator.hpp>

#include <string_view>

#include <glm/gtc/matrix_transform.hpp>

#include <Penrose/Assets/ImageAsset.hpp>
#include <Penrose/Assets/MeshAsset.hpp>
#include <Penrose/Rendering/CommandRecording.hpp>
#include <Penrose/Rendering/Pipeline.hpp>
#include <Penrose/Rendering/PipelineInfo.hpp>
#include <Penrose/Rendering/RenderSubgraph.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

namespace Penrose {

    constexpr static const std::string_view DEFAULT_PIPELINE_INFO_NAME = "DefaultForwardRendering";
    /* TODO: constexpr */ static const PipelineInfo DEFAULT_PIPELINE_INFO = PipelineInfo()
            .setLayout(
                    PipelineLayout()
                            .addConstant(
                                    PipelineLayoutConstant(PipelineShaderStageType::Vertex, 0,
                                                           sizeof(ForwardSceneDrawRenderOperator::RenderData))
                            )
                            .addBinding(
                                    PipelineLayoutBinding(PipelineShaderStageType::Fragment,
                                                          PipelineLayoutBindingType::Sampler,
                                                          1)
                            )
            )
            .addStage(
                    PipelineShaderStage(PipelineShaderStageType::Vertex,
                                        "shaders/default-forward-rendering.vert.asset")
            )
            .addStage(
                    PipelineShaderStage(PipelineShaderStageType::Fragment,
                                        "shaders/default-forward-rendering.frag.asset")
            )
            .addBinding(
                    PipelineBinding(PipelineBindingInputRate::Vertex, sizeof(Vertex))
                            .addAttribute(PipelineBindingAttribute(PipelineBindingAttributeFormat::Vec3,
                                                                   offsetof(Vertex, pos)))
                            .addAttribute(PipelineBindingAttribute(PipelineBindingAttributeFormat::Vec3,
                                                                   offsetof(Vertex, normal)))
                            .addAttribute(PipelineBindingAttribute(PipelineBindingAttributeFormat::Vec3,
                                                                   offsetof(Vertex, color)))
                            .addAttribute(PipelineBindingAttribute(PipelineBindingAttributeFormat::Vec2,
                                                                   offsetof(Vertex, uv)))
            );

    constexpr static const SamplerInfo DEFAULT_SAMPLER_INFO = SamplerInfo()
            .setAddressMode(SamplerAddressMode::Repeat)
            .setMinFilter(SamplerFilter::Linear)
            .setMagFilter(SamplerFilter::Linear)
            .setBorderColor(SamplerBorderColor::Black);

    ForwardSceneDrawRenderOperator::ForwardSceneDrawRenderOperator(ResourceSet *resources)
            : _assetManager(resources->getLazy<AssetManager>()),
              _pipelineFactory(resources->getLazy<PipelineFactory>()),
              _renderListBuilder(resources->getLazy<RenderListBuilder>()),
              _samplerFactory(resources->getLazy<SamplerFactory>()) {
        //
    }

    void ForwardSceneDrawRenderOperator::init() {
        this->_pipelineFactory->addPipeline(std::string(DEFAULT_PIPELINE_INFO_NAME), DEFAULT_PIPELINE_INFO);
        this->_sampler = std::unique_ptr<Sampler>(this->_samplerFactory->makeSampler(DEFAULT_SAMPLER_INFO));
    }

    void ForwardSceneDrawRenderOperator::destroy() {
        this->_sampler = std::nullopt;
    }

    ParamsCollection ForwardSceneDrawRenderOperator::getDefaults() const {
        ParamsCollection params;

        params.setString(PARAM_PIPELINE, std::string(DEFAULT_PIPELINE_INFO_NAME));
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

        for (const auto &drawable: renderList->drawables) {
            auto maybeMesh = this->_assetManager->tryGetAsset<MeshAsset>(drawable.meshAsset);
            auto maybeImage = this->_assetManager->tryGetAsset<ImageAsset>(drawable.albedoTextureAsset);

            if (!maybeMesh.has_value() || !maybeImage.has_value()) {
                continue;
            }

            auto descriptor = pipeline->getDescriptorFor(drawable.entity, {
                    DescriptorBindingValue(0)
                            .setImage(maybeImage->get()->getImage())
                            .setSampler(this->_sampler->get())
            });

            commandRecording->bindDescriptor(pipeline, descriptor);

            auto renderData = RenderData{
                    .matrix = projection * renderList->view.view * drawable.model,
                    .model = drawable.model,
                    .modelRot = drawable.modelRot,
                    .color = drawable.color
            };

            commandRecording->bindPushConstants(pipeline, 0, &renderData);

            commandRecording->draw(maybeMesh->get()->getVertexBuffer(),
                                   maybeMesh->get()->getIndexBuffer());
        }
    }

    glm::mat4 ForwardSceneDrawRenderOperator::getProjection(const RenderOperator::Context &context, View *view) {
        auto &[renderAreaWidth, renderAreaHeight] = context.renderArea;

        if (auto perspective = std::get_if<PerspectiveProjection>(&view->projection)) {
            return glm::perspective(perspective->fov,
                                    static_cast<float>(renderAreaWidth) / static_cast<float>(renderAreaHeight),
                                    perspective->near,
                                    perspective->far);
        }

        if (auto orthographic = std::get_if<OrthographicProjection>(&view->projection)) {
            return glm::ortho(orthographic->left, orthographic->right,
                              orthographic->bottom, orthographic->top,
                              orthographic->near, orthographic->far);
        }

        return glm::mat4(1);
    }
}
