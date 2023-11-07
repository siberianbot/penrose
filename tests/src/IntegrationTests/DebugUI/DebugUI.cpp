#include <catch2/catch_all.hpp>

#include <Penrose/Engine.hpp>
#include <Penrose/Assets/AssetDictionary.hpp>
#include <Penrose/Assets/AssetManager.hpp>
#include <Penrose/Rendering/RenderGraphContext.hpp>
#include <Penrose/Rendering/RenderGraphInfo.hpp>
#include <Penrose/UI/UIManager.hpp>
#include <Penrose/UI/ValueProxy.hpp>

#include <Penrose/Builtin/ImGui/Rendering/ImGuiRenderOperator.hpp>

#include "src/ECS/TestCountdownSystem.hpp"

using namespace Penrose;

TEST_CASE("DebugUI", "[engine-int-test]") {

    class UIContext : public Resource<UIContext, ResourceGroup::Custom>,
                      public Runnable {
    public:
        explicit UIContext(ResourceSet *resources)
                : _uiManager(resources->get<UIManager>()) {
            //
        }

        ~UIContext() override = default;

        void run() override {
            auto buttonPressed = std::make_shared<BooleanValueProxy>(false);

            auto viewModel = ObjectValueProxy()
                    .property<ObjectValueProxy>("root", ObjectValueProxy()
                            .property<StringValueProxy>("text", "Same text in label and input")
                            .property<ActionValueProxy>("button_action",
                                                        [buttonPressed]() {
                                                            buttonPressed->setValue(true);
                                                        })
                            .property("button_pressed", buttonPressed)
                            .property<BooleanValueProxy>("checkbox_visibility", false)
                            .property<BooleanValueProxy>("checkbox_enabled", false)
                            .property<ActionValueProxy>("none_action", []() {})
                            .property<BooleanValueProxy>("none_check", true)
                    );

            this->_uiManager->createUI("TestUI", "layouts/root.asset", std::make_unique<ObjectValueProxy>(viewModel));
        }

        void stop() override {
            this->_uiManager->destroyUI("TestUI");
        }

    private:
        ResourceProxy<UIManager> _uiManager;
    };

    Engine engine;

    engine.resources().add<UIContext, ResourceGroup::Custom>()
            .implements<Runnable>()
            .done();

    auto countdownSystem = engine.resources().add<TestCountdownSystem, ResourceGroup::ECSSystem>()
            .implements<Initializable>()
            .implements<System>()
            .done();

    countdownSystem->setTestTimeout(5.0f);

    auto assetDictionary = engine.resources().get<AssetDictionary>();
    assetDictionary->addDir("tests/data");

    auto assetManager = engine.resources().get<AssetManager>();
    assetManager->enqueue("layouts/root.asset");

    auto uiParams = ParamsCollection();
    uiParams.setString(ImGuiRenderOperator::PARAM_UI_NAME, "TestUI");

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
                                     .setOperatorInfo(RenderOperatorInfo(ImGuiRenderOperator::name(),
                                                                         std::move(uiParams))))
            );

    auto renderContext = engine.resources().get<RenderGraphContext>();
    renderContext->setRenderGraph(graph);

    REQUIRE_NOTHROW([&]() { engine.run(); }());
}
