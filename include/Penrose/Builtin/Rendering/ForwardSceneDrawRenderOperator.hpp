#ifndef PENROSE_BUILTIN_RENDERING_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP
#define PENROSE_BUILTIN_RENDERING_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP

#include <memory>
#include <optional>
#include <string_view>

#include <glm/mat4x4.hpp>

#include <Penrose/Rendering/RenderList.hpp>
#include <Penrose/Rendering/RenderOperator.hpp>
#include <Penrose/Rendering/Sampler.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/Initializable.hpp>

namespace Penrose {

    class ResourceSet;
    class AssetManager;
    class PipelineFactory;
    class RenderListBuilder;
    class SamplerFactory;

    class ForwardSceneDrawRenderOperator : public Resource, public Initializable, public RenderOperator {
    public:
        constexpr static const std::string_view NAME = "ForwardSceneDraw";

        constexpr static const std::string_view PARAM_PIPELINE = "Pipeline";
        constexpr static const std::string_view PARAM_RENDER_LIST = "RenderList";

        explicit ForwardSceneDrawRenderOperator(ResourceSet *resources);
        ~ForwardSceneDrawRenderOperator() override = default;

        void init() override;
        void destroy() override;

        [[nodiscard]] std::string getName() const override { return std::string(NAME); }

        [[nodiscard]] ParamsCollection getDefaults() const override;

        void execute(CommandRecording *commandRecording, const RenderOperator::Context &context) override;

    private:
        AssetManager *_assetManager;
        PipelineFactory *_pipelineFactory;
        RenderListBuilder *_renderListBuilder;
        SamplerFactory *_samplerFactory;

        std::optional<std::unique_ptr<Sampler>> _sampler;

        [[nodiscard]] static glm::mat4 getProjection(const RenderOperator::Context &context, View *view);
    };
}

#endif // PENROSE_BUILTIN_RENDERING_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP
