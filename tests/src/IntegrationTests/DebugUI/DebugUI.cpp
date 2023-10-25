#include <catch2/catch_all.hpp>

#include <Penrose/Engine.hpp>
#include <Penrose/Rendering/RenderGraphContext.hpp>
#include <Penrose/Rendering/RenderGraphInfo.hpp>

#include <Penrose/Builtin/Debug/Rendering/DebugUIDrawRenderOperator.hpp>
#include <Penrose/Builtin/Debug/UI/UIContext.hpp>
#include <Penrose/Builtin/Debug/UI/Widgets.hpp>

#include "src/ECS/TestCountdownSystem.hpp"

using namespace Penrose;

TEST_CASE("DebugUI", "[engine-int-test]") {

    Engine engine;

    auto countdownSystem = engine.resources().add<TestCountdownSystem, ResourceGroup::ECSSystem>()
            .implements<Initializable>()
            .implements<System>()
            .done();

    countdownSystem->setTestTimeout(5.0f);

    auto window = std::shared_ptr<Window>( // NOLINT(modernize-make-shared)
            new Window("Debug UI Window", {
                    std::make_shared<Label>("It's a label")
            })
    );

    engine.resources().get<UIContext>()->setRoot("DebugUIWindow", window);

    auto graph = RenderGraphInfo()
            .setTarget("swapchain", RenderTargetInfo(RenderTargetSource::Swapchain))
            .setSubgraph("default", RenderSubgraphInfo()
                    .addAttachment(RenderAttachmentInfo("swapchain")
                                           .setClearValue(RenderAttachmentClearValueInfo({0, 0, 0, 1}))
                                           .setLoadOp(RenderAttachmentLoadOp::Clear)
                                           .setStoreOp(RenderAttachmentStoreOp::Store)
                                           .setInitialLayout(RenderAttachmentLayout::Undefined)
                                           .setFinalLayout(RenderAttachmentLayout::Present))
                    .addPass(RenderSubgraphPassInfo()
                                     .addColorAttachmentIdx(0)
                                     .setOperatorInfo(RenderOperatorInfo(
                                             std::string(DebugUIDrawRenderOperator::NAME))))
            );

    auto renderContext = engine.resources().get<RenderGraphContext>();
    renderContext->setRenderGraph(graph);

    REQUIRE_NOTHROW([&]() { engine.run(); }());
}
