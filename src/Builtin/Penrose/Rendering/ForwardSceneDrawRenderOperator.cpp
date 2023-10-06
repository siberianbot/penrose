#include <Penrose/Builtin/Penrose/Rendering/ForwardSceneDrawRenderOperator.hpp>

#include <array>
#include <cstdint>
#include <string_view>

#include <glm/gtc/matrix_transform.hpp>

#include <Penrose/Assets/ImageAsset.hpp>
#include <Penrose/Assets/MeshAsset.hpp>
#include <Penrose/Common/Vertex.hpp>
#include <Penrose/Rendering/CommandRecording.hpp>
#include <Penrose/Rendering/DescriptorBindingValue.hpp>
#include <Penrose/Rendering/PipelineInfo.hpp>
#include <Penrose/Rendering/RenderSubgraph.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/Utils/OptionalUtils.hpp>

namespace Penrose {

    constexpr static const std::array<std::byte, 4 * 2 * 2> PLACEHOLDER_TEXTURE_DATA = {
            (std::byte) 0x00, (std::byte) 0x00, (std::byte) 0x00, (std::byte) 0xFF,
            (std::byte) 0xFF, (std::byte) 0x00, (std::byte) 0xFF, (std::byte) 0xFF,
            (std::byte) 0xFF, (std::byte) 0x00, (std::byte) 0xFF, (std::byte) 0xFF,
            (std::byte) 0x00, (std::byte) 0x00, (std::byte) 0x00, (std::byte) 0xFF,
    };

    constexpr static const std::string_view DEFAULT_PIPELINE_INFO_NAME = "DefaultForwardRendering";
    /* TODO: constexpr */ static const PipelineInfo DEFAULT_PIPELINE_INFO = PipelineInfo()
            .setLayout(
                    PipelineLayout()
                            .addConstant(
                                    PipelineLayoutConstant(PipelineShaderStageType::Vertex, 0,
                                                           sizeof(ForwardSceneDrawRenderOperator::PerRenderData))
                            )
                            .addBinding(
                                    PipelineLayoutBinding(PipelineShaderStageType::Fragment,
                                                          PipelineLayoutBindingType::Sampler, TEXTURE_COUNT)
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
                    PipelineBinding(PipelineBindingInputRate::Instance,
                                    sizeof(ForwardSceneDrawRenderOperator::PerInstanceData))
                            .addAttribute(PipelineBindingAttribute(
                                    PipelineBindingAttributeFormat::Mat4,
                                    offsetof(ForwardSceneDrawRenderOperator::PerInstanceData, model))
                            )
                            .addAttribute(PipelineBindingAttribute(
                                    PipelineBindingAttributeFormat::Mat4,
                                    offsetof(ForwardSceneDrawRenderOperator::PerInstanceData, modelRot))
                            )
                            .addAttribute(PipelineBindingAttribute(
                                    PipelineBindingAttributeFormat::Vec3,
                                    offsetof(ForwardSceneDrawRenderOperator::PerInstanceData, color))
                            )
                            .addAttribute(PipelineBindingAttribute(
                                    PipelineBindingAttributeFormat::UInt,
                                    offsetof(ForwardSceneDrawRenderOperator::PerInstanceData, textureId))
                            )
            )
            .addBinding(
                    PipelineBinding(PipelineBindingInputRate::Vertex, sizeof(Vertex))
                            .addAttribute(PipelineBindingAttribute(
                                    PipelineBindingAttributeFormat::Vec3,
                                    offsetof(Vertex, pos))
                            )
                            .addAttribute(PipelineBindingAttribute(
                                    PipelineBindingAttributeFormat::Vec3,
                                    offsetof(Vertex, normal))
                            )
                            .addAttribute(PipelineBindingAttribute(
                                    PipelineBindingAttributeFormat::Vec3,
                                    offsetof(Vertex, color))
                            )
                            .addAttribute(PipelineBindingAttribute(
                                    PipelineBindingAttributeFormat::Vec2,
                                    offsetof(Vertex, uv))
                            )
            );

    constexpr static const SamplerInfo DEFAULT_SAMPLER_INFO = SamplerInfo()
            .setAddressMode(SamplerAddressMode::Repeat)
            .setMinFilter(SamplerFilter::Linear)
            .setMagFilter(SamplerFilter::Linear)
            .setBorderColor(SamplerBorderColor::Black);

    ForwardSceneDrawRenderOperator::ForwardSceneDrawRenderOperator(ResourceSet *resources)
            : _assetManager(resources->getLazy<AssetManager>()),
              _bufferFactory(resources->getLazy<BufferFactory>()),
              _pipelineFactory(resources->getLazy<PipelineFactory>()),
              _renderingObjectManager(resources->getLazy<RenderingObjectManager>()),
              _renderListBuilder(resources->getLazy<RenderListBuilder>()),
              _samplerFactory(resources->getLazy<SamplerFactory>()) {
        //
    }

    void ForwardSceneDrawRenderOperator::init() {
        this->_pipelineFactory->addPipeline(std::string(DEFAULT_PIPELINE_INFO_NAME), DEFAULT_PIPELINE_INFO);

        // TODO: should be configurable
        const std::uint32_t count = 16 * 1024;
        const std::uint32_t size = sizeof(ForwardSceneDrawRenderOperator::PerInstanceData) * count;
        auto instanceDataBuffer = this->_bufferFactory->makeBuffer(BufferType::Vertex, size, count, true);

        this->_instanceDataBuffer = std::unique_ptr<Buffer>(instanceDataBuffer);
        this->_sampler = std::unique_ptr<Sampler>(this->_samplerFactory->makeSampler(DEFAULT_SAMPLER_INFO));

        auto placeholderTexture = this->_renderingObjectManager->makeImage(ImageFormat::RGBA, 2, 2,
                                                                           {PLACEHOLDER_TEXTURE_DATA.begin(),
                                                                            PLACEHOLDER_TEXTURE_DATA.end()});

        this->_placeholderTexture = std::unique_ptr<Image>(placeholderTexture);
    }

    void ForwardSceneDrawRenderOperator::destroy() {
        this->_placeholderTexture = std::nullopt;
        this->_sampler = std::nullopt;
        this->_instanceDataBuffer = std::nullopt;
        this->_descriptorMap.clear();
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

        auto perRenderData = PerRenderData{
                .projectionView = projection * renderList->view.view
        };

        commandRecording->bindPushConstants(pipeline, 0, &perRenderData);

        Descriptor *descriptor;
        auto descriptorIt = this->_descriptorMap.find(pipeline);

        if (descriptorIt != this->_descriptorMap.end()) {
            descriptor = descriptorIt->second.get();
        } else {
            descriptor = this->_descriptorMap.emplace(pipeline, pipeline->allocateDescriptor()).first->second.get();
        }

        auto images = std::vector<Image *>(TEXTURE_COUNT, this->_placeholderTexture->get());
        auto samplers = std::vector<Sampler *>(TEXTURE_COUNT, this->_sampler->get());
        for (std::uint32_t idx = 0; idx < renderList->textureCount; idx++) {
            auto &texture = renderList->textures.at(idx);

            auto maybeImage = map(this->_assetManager->tryGetAsset<ImageAsset>(texture.asset),
                                  [](const std::shared_ptr<ImageAsset> &asset) {
                                      return asset->getImage();
                                  });

            images.at(idx) = maybeImage.value_or(this->_placeholderTexture->get());
        }

        descriptor->updateBindingValues({
                                                DescriptorBindingValue(0)
                                                        .setImages(images)
                                                        .setSamplers(samplers)
                                        });

        commandRecording->bindDescriptor(pipeline, descriptor);

        std::uint32_t totalInstanceIdx = 0;
        for (const auto &mesh: renderList->meshes) {
            auto maybeMesh = this->_assetManager->tryGetAsset<MeshAsset>(mesh.asset);

            if (!maybeMesh.has_value()) {
                continue;
            }

            auto data = reinterpret_cast<PerInstanceData *>((*this->_instanceDataBuffer)->getPointer());

            commandRecording->bindBuffer(0, (*this->_instanceDataBuffer).get(), totalInstanceIdx);

            for (const auto &meshInstance: mesh.instances) {
                data[totalInstanceIdx].model = meshInstance.model;
                data[totalInstanceIdx].modelRot = meshInstance.modelRot;
                data[totalInstanceIdx].color = meshInstance.color;
                data[totalInstanceIdx].textureId = meshInstance.albedoTextureId;

                totalInstanceIdx++;
            }

            commandRecording->bindBuffer(1, maybeMesh->get()->getVertexBuffer(), 0);
            commandRecording->bindIndexBuffer(maybeMesh->get()->getIndexBuffer(), 0);

            commandRecording->draw(maybeMesh->get()->getIndexBuffer()->getCount(), mesh.instances.size());
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
