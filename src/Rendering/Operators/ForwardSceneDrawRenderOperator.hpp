#ifndef PENROSE_RENDERING_OPERATORS_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP
#define PENROSE_RENDERING_OPERATORS_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP

#include <map>
#include <string_view>
#include <unordered_map>

#include "src/Assets/Asset.hpp"
#include "src/Assets/AssetId.hpp"
#include "src/Rendering/Utils.hpp"
#include "src/Rendering/Operators/RenderOperatorFactory.hpp"

namespace Penrose {

    class ResourceSet;
    class AssetManager;
    class DeviceContext;

    class ForwardSceneDrawRenderOperator : public RenderOperator {
    public:
        static constexpr const std::string_view NAME = "ForwardSceneDraw";

        static constexpr const std::string_view PARAM_DEFAULT_VERTEX_SHADER = "DefaultVertexShader";
        static constexpr const std::string_view PARAM_DEFAULT_VERTEX_SHADER_VALUE = "shaders/default-forward-rendering.vert.spv";
        static constexpr const std::string_view PARAM_DEFAULT_FRAGMENT_SHADER = "DefaultFragmentShader";
        static constexpr const std::string_view PARAM_DEFAULT_FRAGMENT_SHADER_VALUE = "shaders/default-forward-rendering.frag.spv";

    private:
        AssetManager *_assetManager;
        DeviceContext *_deviceContext;
        PipelineLayout _pipelineLayout;
        Pipeline _pipeline;

        std::unordered_map<AssetId, MeshAsset> _meshes;

        [[nodiscard]] MeshAsset *getMesh(const AssetId &asset);

    public:
        ForwardSceneDrawRenderOperator(AssetManager *assetManager,
                                       DeviceContext *deviceContext,
                                       PipelineLayout &&pipelineLayout,
                                       Pipeline &&pipeline);
        ~ForwardSceneDrawRenderOperator() override = default;

        void execute(const RenderOperatorExecutionContext &context) override;

        [[nodiscard]] static std::unique_ptr<RenderOperator> make(const RenderOperatorFactoryContext &context);
    };
}

#endif // PENROSE_RENDERING_OPERATORS_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP
