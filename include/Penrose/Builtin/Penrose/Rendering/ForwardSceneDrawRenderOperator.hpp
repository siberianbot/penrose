#ifndef PENROSE_BUILTIN_PENROSE_RENDERING_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP
#define PENROSE_BUILTIN_PENROSE_RENDERING_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string_view>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/Rendering/BufferFactory.hpp>
#include <Penrose/Rendering/Descriptor.hpp>
#include <Penrose/Rendering/Image.hpp>
#include <Penrose/Rendering/ImageFactory.hpp>
#include <Penrose/Rendering/Pipeline.hpp>
#include <Penrose/Rendering/PipelineFactory.hpp>
#include <Penrose/Rendering/RenderingObjectManager.hpp>
#include <Penrose/Rendering/RenderList.hpp>
#include <Penrose/Rendering/RenderListBuilder.hpp>
#include <Penrose/Rendering/RenderOperator.hpp>
#include <Penrose/Rendering/Sampler.hpp>
#include <Penrose/Rendering/SamplerFactory.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

namespace Penrose {

    class ResourceSet;

    class ForwardSceneDrawRenderOperator : public Resource<ForwardSceneDrawRenderOperator>,
                                           public Initializable,
                                           public RenderOperator {
    public:
        constexpr static const std::string_view NAME = "ForwardSceneDraw";

        constexpr static const std::string_view PARAM_PIPELINE = "Pipeline";
        constexpr static const std::string_view PARAM_RENDER_LIST = "RenderList";

        struct PerRenderData {
            glm::mat4 projectionView;
        };

        struct PerInstanceData {
            glm::mat4 model;
            glm::mat4 modelRot;
            glm::vec3 color;
            std::uint32_t textureId;
        };

        explicit ForwardSceneDrawRenderOperator(ResourceSet *resources);
        ~ForwardSceneDrawRenderOperator() override = default;

        void init() override;
        void destroy() override;

        [[nodiscard]] std::string getName() const override { return std::string(NAME); }

        [[nodiscard]] ParamsCollection getDefaults() const override;

        void execute(CommandRecording *commandRecording, const RenderOperator::Context &context) override;

    private:
        ResourceProxy<AssetManager> _assetManager;
        ResourceProxy<BufferFactory> _bufferFactory;
        ResourceProxy<PipelineFactory> _pipelineFactory;
        ResourceProxy<RenderingObjectManager> _renderingObjectManager;
        ResourceProxy<RenderListBuilder> _renderListBuilder;
        ResourceProxy<SamplerFactory> _samplerFactory;

        std::optional<std::unique_ptr<Buffer>> _instanceDataBuffer;
        std::optional<std::unique_ptr<Image>> _placeholderTexture;
        std::optional<std::unique_ptr<Sampler>> _sampler;

        std::map<Pipeline *, std::unique_ptr<Descriptor>> _descriptorMap;

        [[nodiscard]] static glm::mat4 getProjection(const RenderOperator::Context &context, View *view);
    };
}

#endif // PENROSE_BUILTIN_PENROSE_RENDERING_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP
