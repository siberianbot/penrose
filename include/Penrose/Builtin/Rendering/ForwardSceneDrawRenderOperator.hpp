#ifndef PENROSE_BUILTIN_RENDERING_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP
#define PENROSE_BUILTIN_RENDERING_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP

#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <glm/mat4x4.hpp>

#include <Penrose/ECS/Entity.hpp>
#include <Penrose/Rendering/RenderList.hpp>
#include <Penrose/Rendering/RenderOperator.hpp>
#include <Penrose/Rendering/RenderOperatorFactory.hpp>

namespace Penrose {

    class ResourceSet;
    class AssetManager;
    class DeviceContext;
    class PipelineFactory;
    class RenderListBuilder;
    class VkPipeline;

    class ForwardSceneDrawRenderOperator : public RenderOperator {
    public:
        static constexpr const std::string_view NAME = "ForwardSceneDraw";

        static constexpr const std::string_view PIPELINE_PARAM = "Pipeline";
        static constexpr const std::string_view RENDER_LIST_PARAM = "RenderList";

        ForwardSceneDrawRenderOperator(AssetManager *assetManager,
                                       DeviceContext *deviceContext,
                                       RenderListBuilder *renderListBuilder,
                                       VkPipeline *pipeline,
                                       vk::Sampler sampler,
                                       std::string renderList);
        ~ForwardSceneDrawRenderOperator() override;

        void execute(const RenderOperator::Context &context) override;

    private:
        AssetManager *_assetManager;
        DeviceContext *_deviceContext;
        RenderListBuilder *_renderListBuilder;
        VkPipeline *_pipeline;
        vk::Sampler _sampler;
        std::string _renderList;

        std::unordered_map<Entity, std::vector<vk::DescriptorSet>> _descriptors;
        [[nodiscard]] std::optional<vk::DescriptorSet> tryGetDescriptorSet(const Entity &entity,
                                                                           const std::uint32_t &frameIdx,
                                                                           const std::string &asset);

        [[nodiscard]] static glm::mat4 getProjection(const RenderOperator::Context &context, View *view);
    };

    class ForwardSceneDrawRenderOperatorFactory : public RenderOperatorFactory {
    public:
        explicit ForwardSceneDrawRenderOperatorFactory(ResourceSet *resources);
        ~ForwardSceneDrawRenderOperatorFactory() override = default;

        [[nodiscard]] std::string_view name() const override {
            return ForwardSceneDrawRenderOperator::NAME;
        }

        [[nodiscard]] ParamsCollection defaults() const override;

        [[nodiscard]] RenderOperator *create(const RenderOperatorFactory::Context &context) const override;

    private:
        AssetManager *_assetManager;
        DeviceContext *_deviceContext;
        PipelineFactory *_pipelineFactory;
        RenderListBuilder *_renderListBuilder;
    };
}

#endif // PENROSE_BUILTIN_RENDERING_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP
