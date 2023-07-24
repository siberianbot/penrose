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
#include <Penrose/Rendering/Utils.hpp>

namespace Penrose {

    class ResourceSet;
    class AssetManager;
    class DeviceContext;
    class RenderContext;

    class ForwardSceneDrawRenderOperator : public RenderOperator {
    public:
        static constexpr const std::string_view NAME = "ForwardSceneDraw";

        static constexpr const std::string_view RENDER_LIST_PARAM = "RenderList";
        static constexpr const std::string_view VERTEX_SHADER_PARAM = "VertexShader";
        static constexpr const std::string_view FRAGMENT_SHADER_PARAM = "FragmentShader";

        ForwardSceneDrawRenderOperator(AssetManager *assetManager,
                                       DeviceContext *deviceContext,
                                       RenderContext *renderContext,
                                       DescriptorSetLayout descriptorSetLayout,
                                       PipelineLayout pipelineLayout,
                                       Pipeline pipeline,
                                       Sampler sampler,
                                       std::string renderList);
        ~ForwardSceneDrawRenderOperator() override = default;

        void execute(const RenderOperator::Context &context) override;

    private:
        AssetManager *_assetManager;
        DeviceContext *_deviceContext;
        RenderContext *_renderContext;
        DescriptorSetLayout _descriptorSetLayout;
        PipelineLayout _pipelineLayout;
        Pipeline _pipeline;
        Sampler _sampler;
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
        RenderContext *_renderContext;
    };
}

#endif // PENROSE_BUILTIN_RENDERING_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP
