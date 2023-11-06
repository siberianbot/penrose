#include <catch2/catch_all.hpp>

#include <Penrose/Engine.hpp>
#include <Penrose/Common/Log.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Events/InputEvents.hpp>
#include <Penrose/Rendering/RenderGraphContext.hpp>
#include <Penrose/Rendering/RenderGraphInfo.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/UI/UIManager.hpp>
#include <Penrose/UI/ValueProxy.hpp>

#include <Penrose/Builtin/ImGui/Rendering/ImGuiRenderOperator.hpp>

using namespace Penrose;

TEST_CASE("ImGuiInputHook", "[engine-interactive-test]") {

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
                                                        [&buttonPressed]() {
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

    constexpr static std::string_view TEST_LOGIC_SYSTEM_TAG = "InputReceiving";

    class TestLogicSystem : public Resource<TestLogicSystem, ResourceGroup::ECSSystem>,
                            public Initializable,
                            public System {
    public:
        explicit TestLogicSystem(ResourceSet *resources)
                : _eventQueue(resources->get<InputEventQueue>()),
                  _log(resources->get<Log>()) {
            //
        }

        ~TestLogicSystem() override = default;

        void init() override {
            this->_eventQueue->addHandler<KeyStateUpdatedEvent>([this](const KeyStateUpdatedEvent *event) {
                this->_log->writeDebug(TEST_LOGIC_SYSTEM_TAG,
                                       "Key state updated - key {:#x}, state {:#x}",
                                       static_cast<std::uint32_t>(event->getKey()),
                                       static_cast<std::uint32_t>(event->getState()));
            });

            this->_eventQueue->addHandler<MouseMovementEvent>([this](const MouseMovementEvent *event) {
                auto [x, y] = event->getPosition();
                auto [dx, dy] = event->getDelta();

                this->_log->writeDebug(TEST_LOGIC_SYSTEM_TAG,
                                       "Mouse moved - x = {}, y = {}, dx = {}, dy = {}", x, y, dx, dy);
            });

            this->_eventQueue->addHandler<MouseScrollEvent>([this](const MouseScrollEvent *event) {
                auto [dx, dy] = event->getOffset();

                this->_log->writeDebug(TEST_LOGIC_SYSTEM_TAG,
                                       "Scroll - dx = {}, dy = {}", dx, dy);
            });
        }

        void update(float) override { /* nothing to do */ }

        void destroy() override { /* nothing to do */ }

        [[nodiscard]] std::string getName() const override { return "TestLogic"; }

    private:
        ResourceProxy<InputEventQueue> _eventQueue;
        ResourceProxy<Log> _log;
    };

    Engine engine;

    engine.resources().add<UIContext, ResourceGroup::Custom>()
            .implements<Runnable>()
            .done();

    engine.resources().add<TestLogicSystem, ResourceGroup::ECSSystem>()
            .implements<Initializable>()
            .implements<System>()
            .done();

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
