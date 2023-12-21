#ifndef PENROSE_RENDERING_DEFAULT_RENDERER_HPP
#define PENROSE_RENDERING_DEFAULT_RENDERER_HPP

#include <memory>

#include <Penrose/Rendering/CommandRecorder.hpp>
#include <Penrose/Rendering/Objects/Image.hpp>
#include <Penrose/Rendering/Objects/ImageFactory.hpp>
#include <Penrose/Rendering/Objects/Pipeline.hpp>
#include <Penrose/Rendering/Objects/PipelineFactory.hpp>
#include <Penrose/Rendering/Objects/Sampler.hpp>
#include <Penrose/Rendering/Objects/SamplerFactory.hpp>
#include <Penrose/Rendering/Renderer.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    class DefaultRenderer final: public Resource<DefaultRenderer>,
                                 public Renderer {
    public:
        static constexpr std::uint32_t TEXTURE_COUNT = 64;

        struct ProjectionConstant {
            // TODO
        };

        struct InstanceInput {
            // TODO
        };

        struct VertexInput {
            // TODO
        };

        explicit DefaultRenderer(const ResourceSet *resources);
        ~DefaultRenderer() override = default;

        [[nodiscard]] std::string getName() const override { return "Default"; }

        void init() override;
        void destroy() override;

        void setRenderGraph(GraphInfo &&graphInfo) override { this->_graph = graphInfo; }

        [[nodiscard]] const GraphInfo &getRenderGraph() override { return this->_graph; }

        void execute(RendererContext *context, const Params &params) override;

    private:
        ResourceProxy<ImageFactory> _imageFactory;
        ResourceProxy<PipelineFactory> _pipelineFactory;
        ResourceProxy<SamplerFactory> _samplerFactory;

        GraphInfo _graph;

        std::unique_ptr<Image> _placeholder;
        std::unique_ptr<Pipeline> _pipeline;
        std::unique_ptr<Sampler> _sampler;

        ProjectionConstant _projection;

        void draw(CommandRecorder *commandRecorder);
    };
}

#endif // PENROSE_RENDERING_DEFAULT_RENDERER_HPP
