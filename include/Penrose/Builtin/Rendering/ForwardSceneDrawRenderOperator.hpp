#ifndef PENROSE_BUILTIN_RENDERING_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP
#define PENROSE_BUILTIN_RENDERING_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP

#include <map>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <Penrose/Assets/Asset.hpp>
#include <Penrose/Assets/AssetId.hpp>
#include <Penrose/ECS/ECSBase.hpp>
#include <Penrose/Rendering/RenderOperator.hpp>
#include <Penrose/Rendering/Utils.hpp>

namespace Penrose {

    class ResourceSet;
    class AssetManager;
    class DeviceContext;
    class RenderContext;

    class ForwardSceneDrawRenderOperator : public RenderOperator {
    public:
        static constexpr const std::string_view RENDER_LIST_PARAM = "RenderList";
        static constexpr const std::string_view VERTEX_SHADER_PARAM = "VertexShader";
        static constexpr const std::string_view FRAGMENT_SHADER_PARAM = "FragmentShader";

    private:
        AssetManager *_assetManager;
        DeviceContext *_deviceContext;
        RenderContext *_renderContext;
        DescriptorSetLayout _descriptorSetLayout;
        PipelineLayout _pipelineLayout;
        Pipeline _pipeline;
        Sampler _sampler;
        std::string _renderList;

        std::unordered_map<AssetId, MeshAsset> _meshes;
        std::unordered_map<AssetId, ImageAsset> _images;
        std::unordered_map<Entity, std::vector<vk::DescriptorSet>> _descriptors;

        [[nodiscard]] MeshAsset *getMesh(const AssetId &asset);
        [[nodiscard]] ImageAsset *getImage(const AssetId &asset);
        [[nodiscard]] vk::DescriptorSet getDescriptorSet(const Entity &entity,
                                                         const std::uint32_t &frameIdx,
                                                         const AssetId &assetId);

    public:
        ForwardSceneDrawRenderOperator(AssetManager *assetManager,
                                       DeviceContext *deviceContext,
                                       RenderContext *renderContext,
                                       DescriptorSetLayout descriptorSetLayout,
                                       PipelineLayout pipelineLayout,
                                       Pipeline pipeline,
                                       Sampler sampler,
                                       std::string renderList);
        ~ForwardSceneDrawRenderOperator() override = default;

        void execute(const RenderOperatorExecutionContext &context) override;

        [[nodiscard]] static std::string name() { return "ForwardSceneDraw"; }

        [[nodiscard]] static ParamsCollection defaults();

        [[nodiscard]] static std::unique_ptr<RenderOperator> create(const RenderOperatorCreateContext &context);
    };
}

#endif // PENROSE_BUILTIN_RENDERING_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP
