#include "DefaultRenderer.hpp"

namespace Penrose {

    static constexpr std::array PLACEHOLDER_IMAGE_DATA = {
        static_cast<std::byte>(0x00), static_cast<std::byte>(0x00), static_cast<std::byte>(0x00),
        static_cast<std::byte>(0xFF), static_cast<std::byte>(0xFF), static_cast<std::byte>(0x00),
        static_cast<std::byte>(0xFF), static_cast<std::byte>(0xFF), static_cast<std::byte>(0xFF),
        static_cast<std::byte>(0x00), static_cast<std::byte>(0xFF), static_cast<std::byte>(0xFF),
        static_cast<std::byte>(0x00), static_cast<std::byte>(0x00), static_cast<std::byte>(0x00),
        static_cast<std::byte>(0xFF),
    };

    DefaultRenderer::DefaultRenderer(const ResourceSet *resources)
        : _imageFactory(resources->get<ImageFactory>()),
          _pipelineFactory(resources->get<PipelineFactory>()),
          _samplerFactory(resources->get<SamplerFactory>()),
          _graph(GraphInfo {
              .name = "DefaultRendererGraph",
              .attachments =
                  {AttachmentInfo {
                       .format = std::nullopt,
                       .clearValue = {.color = {0, 0, 0, 1}},
                       .loadOp = AttachmentLoadOp::Load,
                       .storeOp = AttachmentStoreOp::Store,
                       .initialLayout = AttachmentLayout::Undefined,
                       .finalLayout = AttachmentLayout::Present,
                   }, AttachmentInfo {
                       .format = RenderFormat::D32Float,
                       .clearValue = {.depth = 1},
                       .loadOp = AttachmentLoadOp::Clear,
                       .storeOp = AttachmentStoreOp::Store,
                       .initialLayout = AttachmentLayout::Undefined,
                       .finalLayout = AttachmentLayout::DepthStencilAttachment,
                   }},
              .passes = {PassInfo {
                  .dependsOn = {},
                  .colorTargets = {0},
                  .depthStencilTarget = {1},
                  .function = "Draw",
              }},
              .area = std::nullopt,
    }) {
        //
    }

    void DefaultRenderer::init() {
        const auto placeholder = this->_imageFactory->makeImage(
            ImageFormat::RGBA, 2, 2, {PLACEHOLDER_IMAGE_DATA.begin(), PLACEHOLDER_IMAGE_DATA.end()}
        );

        const auto pipeline = this->_pipelineFactory->makePipeline({
            .name = "DefaultRendererPipeline",
            .shaders =
                {
                          PipelineShader {
                        .stage = PipelineShaderStage::Vertex,
                        .shaderAssetName = "shaders/default/vert",
                    }, PipelineShader {
                        .stage = PipelineShaderStage::Fragment,
                        .shaderAssetName = "shaders/default/frag",
                    }, },
            .inputs =
                {
                          PipelineInput {
                        .rate = PipelineInputRate::Instance,
                        .stride = sizeof(InstanceInput),
                        .attributes = {/* TODO */},
                    }, PipelineInput {
                        .rate = PipelineInputRate::Vertex,
                        .stride = sizeof(VertexInput),
                        .attributes = {/* TODO */},
                    }, },
            .constants =
                {
                          PipelineConstant {
                        .stage = PipelineShaderStage::Vertex,
                        .offset = 0,
                        .size = sizeof(ProjectionConstant),
                    }, },
            .objects =
                {
                          PipelineObject {
                        .stage = PipelineShaderStage::Fragment,
                        .type = PipelineObjectType::Sampler,
                        .count = TEXTURE_COUNT,
                    }, },
        });

        const auto sampler = this->_samplerFactory->makeSampler(
            SamplerAddressMode::Repeat, SamplerFilteringMode::Linear, SamplerFilteringMode::Linear,
            SamplerBorderColor::Black
        );

        this->_placeholder = std::unique_ptr<Image>(placeholder);
        this->_pipeline = std::unique_ptr<Pipeline>(pipeline);
        this->_sampler = std::unique_ptr<Sampler>(sampler);
    }

    void DefaultRenderer::destroy() {
        this->_sampler.reset();
        this->_pipeline.reset();
        this->_placeholder.reset();
    }

    void DefaultRenderer::execute(RendererContext *context, const Params &params) {
        const auto targets = {
            TargetInfo {.type = TargetType::Swapchain},
            TargetInfo {
                        .type = TargetType::Image,
                        .name = "depth",
                        .usage = static_cast<TargetUsageFlags>(TargetUsage::DepthStencil),
                        .format = RenderFormat::D32Float
            },
        };

        const auto functions = RendererFunctionMap {
            {"Draw", [&](CommandRecorder *recorder) { this->draw(recorder); }}
        };

        context->executeGraph(targets, this->_graph, functions);
    }

    void DefaultRenderer::draw(CommandRecorder *commandRecorder) {
        // TODO

        commandRecorder->configureViewport(FloatRect(0, 0, 1, 1));
        commandRecorder->configureScissor(FloatRect(0, 0, 1, 1));

        commandRecorder->bindPipeline(
            this->_pipeline.get(),
            PipelineBindingInfo {
                .tag = "DefaultRendererImageBinding",
                .constants = {PipelineConstantBinding {.data = &this->_projection}},
                .objects = {PipelineObjectBinding {
                    .images = std::vector(TEXTURE_COUNT, this->_placeholder.get()), // TODO
                    .samplers = std::vector(TEXTURE_COUNT, this->_sampler.get()),
                }},
            }
        );

        commandRecorder->draw(DrawInfo {
            .count = 0,
            .instanceCount = 0,
            .buffers =
                {
                    // TODO
                },
            .index =
                BufferUsage {
                    // TODO
                },
        });
    }
}
