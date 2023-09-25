#ifndef PENROSE_BUILTIN_PENROSE_RENDERING_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP
#define PENROSE_BUILTIN_PENROSE_RENDERING_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP

#include <memory>
#include <optional>
#include <string_view>

#include <glm/mat4x4.hpp>

#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/Rendering/PipelineFactory.hpp>
#include <Penrose/Rendering/RenderList.hpp>
#include <Penrose/Rendering/RenderListBuilder.hpp>
#include <Penrose/Rendering/RenderOperator.hpp>
#include <Penrose/Rendering/Sampler.hpp>
#include <Penrose/Rendering/SamplerFactory.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/Initializable.hpp>

namespace Penrose {

    class ResourceSet;

    class ForwardSceneDrawRenderOperator : public Resource, public Initializable, public RenderOperator {
    public:
        constexpr static const std::string_view NAME = "ForwardSceneDraw";

        constexpr static const std::string_view PARAM_PIPELINE = "Pipeline";
        constexpr static const std::string_view PARAM_RENDER_LIST = "RenderList";

        struct RenderData {
            alignas(16) glm::mat4 matrix;
            alignas(16) glm::mat4 model;
            alignas(16) glm::mat4 modelRot;
        };

        explicit ForwardSceneDrawRenderOperator(ResourceSet *resources);
        ~ForwardSceneDrawRenderOperator() override = default;

        void init() override;
        void destroy() override;

        [[nodiscard]] std::string getName() const override { return std::string(NAME); }

        [[nodiscard]] ParamsCollection getDefaults() const override;

        void execute(CommandRecording *commandRecording, const RenderOperator::Context &context) override;

    private:
        Lazy<AssetManager> _assetManager;
        Lazy<PipelineFactory> _pipelineFactory;
        Lazy<RenderListBuilder> _renderListBuilder;
        Lazy<SamplerFactory> _samplerFactory;

        std::optional<std::unique_ptr<Sampler>> _sampler;

        [[nodiscard]] static glm::mat4 getProjection(const RenderOperator::Context &context, View *view);
    };
}

#endif // PENROSE_BUILTIN_PENROSE_RENDERING_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP
