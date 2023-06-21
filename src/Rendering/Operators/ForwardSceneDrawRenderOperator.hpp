#ifndef PENROSE_RENDERING_OPERATORS_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP
#define PENROSE_RENDERING_OPERATORS_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP

#include <map>
#include <string_view>
#include <unordered_map>

#include "src/Assets/Asset.hpp"
#include "src/Assets/AssetId.hpp"
#include "src/Rendering/Utils.hpp"
#include "src/Rendering/Operators/RenderOperator.hpp"

namespace Penrose {

    class ResourceSet;
    class AssetManager;

    class ForwardSceneDrawRenderOperator : public RenderOperator {
    public:
        static constexpr const std::string_view VERTEX_SHADER_PARAM = "VertexShader";
        static constexpr const std::string_view FRAGMENT_SHADER_PARAM = "FragmentShader";

    private:
        AssetManager *_assetManager;
        PipelineLayout _pipelineLayout;
        Pipeline _pipeline;

        std::unordered_map<AssetId, MeshAsset> _meshes;

        [[nodiscard]] MeshAsset *getMesh(const AssetId &asset);

    public:
        ForwardSceneDrawRenderOperator(AssetManager *assetManager,
                                       PipelineLayout &&pipelineLayout,
                                       Pipeline &&pipeline);
        ~ForwardSceneDrawRenderOperator() override = default;

        void execute(const RenderOperatorExecutionContext &context) override;

        [[nodiscard]] static std::string name() { return "ForwardSceneDraw"; }

        [[nodiscard]] static RenderOperatorParams defaults();

        [[nodiscard]] static std::unique_ptr<RenderOperator> create(const RenderOperatorCreateContext &context);
    };
}

#endif // PENROSE_RENDERING_OPERATORS_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP
