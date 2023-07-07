#ifndef PENROSE_RENDERING_OPERATORS_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP
#define PENROSE_RENDERING_OPERATORS_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP

#include <map>
#include <string>
#include <string_view>
#include <unordered_map>

#include <Penrose/Assets/AssetId.hpp>
#include <Penrose/Rendering/RenderOperator.hpp>

#include "src/Assets/Asset.hpp"
#include "src/Rendering/Utils.hpp"

namespace Penrose {

    class ResourceSet;
    class AssetManager;
    class RenderContext;

    class ForwardSceneDrawRenderOperator : public RenderOperator {
    public:
        static constexpr const std::string_view RENDER_LIST_PARAM = "RenderList";
        static constexpr const std::string_view VERTEX_SHADER_PARAM = "VertexShader";
        static constexpr const std::string_view FRAGMENT_SHADER_PARAM = "FragmentShader";

    private:
        AssetManager *_assetManager;
        RenderContext *_renderContext;
        PipelineLayout _pipelineLayout;
        Pipeline _pipeline;
        std::string _renderList;

        std::unordered_map<AssetId, MeshAsset> _meshes;

        [[nodiscard]] MeshAsset *getMesh(const AssetId &asset);

    public:
        ForwardSceneDrawRenderOperator(AssetManager *assetManager,
                                       RenderContext *renderContext,
                                       PipelineLayout &&pipelineLayout,
                                       Pipeline &&pipeline,
                                       std::string &&renderList);
        ~ForwardSceneDrawRenderOperator() override = default;

        void execute(const RenderOperatorExecutionContext &context) override;

        [[nodiscard]] static std::string name() { return "ForwardSceneDraw"; }

        [[nodiscard]] static ParamsCollection defaults();

        [[nodiscard]] static std::unique_ptr<RenderOperator> create(const RenderOperatorCreateContext &context);
    };
}

#endif // PENROSE_RENDERING_OPERATORS_FORWARD_SCENE_DRAW_RENDER_OPERATOR_HPP
