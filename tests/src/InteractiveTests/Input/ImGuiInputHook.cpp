#include <catch2/catch_all.hpp>

#include <Penrose/Common/Log.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Engine.hpp>
#include <Penrose/Events/EngineEvents.hpp>
#include <Penrose/Events/InputEvents.hpp>
#include <Penrose/Rendering/RenderGraphContext.hpp>
#include <Penrose/Rendering/RenderGraphInfo.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/UI/UIManager.hpp>
#include <Penrose/UI/Value.hpp>

#include <Penrose/Builtin/ImGui/Rendering/ImGuiRenderOperator.hpp>

using namespace Penrose;

TEST_CASE("ImGuiInputHook", "[engine-interactive-test]") {

    class UIContext: public Resource<UIContext, ResourceGroup::Custom>,
                     public Runnable {
    public:
        explicit UIContext(ResourceSet *resources)
            : _uiManager(resources->get<UIManager>()),
              _engineEventQueue(resources->get<EngineEventQueue>()) {
            //
        }

        ~UIContext() override = default;

        void run() override {
            auto listItems = std::make_shared<ListValue<ObjectValue>>(
                ListValue<ObjectValue>()
                    .push(std::make_shared<ObjectValue>(ObjectValue().property<StringValue>("value", "Item 1")))
                    .push(std::make_shared<ObjectValue>(ObjectValue().property<StringValue>("value", "Item 2")))
                    .push(std::make_shared<ObjectValue>(ObjectValue().property<StringValue>("value", "Item 3")))
            );

            auto listSelected = std::make_shared<IntegerValue>(-1);

            auto buttonPressed = std::make_shared<BooleanValue>(false);

            auto viewModel = std::make_shared<ObjectValue>(
                ObjectValue()
                    .property<ObjectValue>(
                        "root",
                        ObjectValue()
                            .property<BooleanValue>("menu_checked", false)
                            .property<BooleanValue>("menu_enabled", true)
                            .property<ActionValue>(
                                "close",
                                [this]() { this->_engineEventQueue->push<EngineDestroyRequestedEvent>(); }
                            )
                            .property<StringValue>("text", "Same text in label and input")
                            .property<ActionValue>(
                                "button_action",
                                [buttonPressed]() { buttonPressed->setValue(true); }
                            )
                            .property("button_pressed", buttonPressed)
                            .property<BooleanValue>("checkbox_visibility", false)
                            .property<BooleanValue>("checkbox_enabled", false)
                            .property<ActionValue>("none_action", []() {})
                            .property<BooleanValue>("none_check", true)
                            .property("list_items", listItems)
                            .property("list_selected", listSelected)
                            .property<StringValue>(
                                "list_selected_label",
                                [listSelected]() {
                                    return fmt::format("Selected item index: {}", listSelected->getValue());
                                },
                                [](std::string) {}
                            )
                            .property<ActionValue>(
                                "add_item",
                                [listItems]() {
                                    listItems->push(std::make_shared<ObjectValue>(ObjectValue().property<StringValue>(
                                        "value",
                                        fmt::format("Item {}", listItems->getItems().size() + 1)
                                    )));
                                }
                            )
                            .property<BooleanValue>(
                                "remove_item_enabled",
                                [listSelected, listItems]() {
                                    const auto currentValue = listSelected->getValue();
                                    return currentValue >= 0 && currentValue < listItems->getItems().size();
                                },
                                [](bool) {}
                            )
                            .property<ActionValue>(
                                "remove_item",
                                [listSelected, listItems]() {
                                    const auto currentValue = listSelected->getValue();

                                    listItems->getItems().erase(std::next(listItems->getItems().begin(), currentValue));
                                }
                            )
                            .property<ListValue<StringValue>>(
                                "select_items",
                                ListValue<StringValue>().push("Item 1").push("Item 2").push("Item 3").push(
                                    "One more thing"
                                )
                            )
                            .property<IntegerValue>("select_selected", -1)
                    )
                    .property<BooleanValue>("root_opened", true)
            );

            this->_uiManager->createContext("TestUI", "layouts/root.asset", std::move(viewModel));
        }

        void stop() override { this->_uiManager->destroyContext("TestUI"); }

    private:
        ResourceProxy<UIManager> _uiManager;
        ResourceProxy<EngineEventQueue> _engineEventQueue;
    };

    constexpr static std::string_view TEST_LOGIC_SYSTEM_TAG = "InputReceiving";

    class TestLogicSystem: public Resource<TestLogicSystem, ResourceGroup::ECSSystem>,
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
                this->_log->writeDebug(
                    TEST_LOGIC_SYSTEM_TAG,
                    "Key state updated - key {:#x}, state {:#x}",
                    static_cast<std::uint32_t>(event->getKey()),
                    static_cast<std::uint32_t>(event->getState())
                );
            });

            this->_eventQueue->addHandler<MouseMovementEvent>([this](const MouseMovementEvent *event) {
                auto [x, y] = event->getPosition();
                auto [dx, dy] = event->getDelta();

                this->_log
                    ->writeDebug(TEST_LOGIC_SYSTEM_TAG, "Mouse moved - x = {}, y = {}, dx = {}, dy = {}", x, y, dx, dy);
            });

            this->_eventQueue->addHandler<MouseScrollEvent>([this](const MouseScrollEvent *event) {
                auto [dx, dy] = event->getOffset();

                this->_log->writeDebug(TEST_LOGIC_SYSTEM_TAG, "Scroll - dx = {}, dy = {}", dx, dy);
            });
        }

        void update(float) override { /* nothing to do */
        }

        void destroy() override { /* nothing to do */
        }

        [[nodiscard]] std::string getName() const override { return "TestLogic"; }

    private:
        ResourceProxy<InputEventQueue> _eventQueue;
        ResourceProxy<Log> _log;
    };

    Engine engine;

    engine.resources().add<UIContext, ResourceGroup::Custom>().implements<Runnable>().done();

    engine.resources()
        .add<TestLogicSystem, ResourceGroup::ECSSystem>()
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
                     .setSubgraph(
                         "default",
                         RenderSubgraphInfo()
                             .addAttachment(RenderAttachmentInfo("swapchain")
                                                .setClearValue(RenderAttachmentClearValueInfo({0, 0, 0, 1}))
                                                .setLoadOp(RenderAttachmentLoadOp::Clear)
                                                .setStoreOp(RenderAttachmentStoreOp::Store)
                                                .setInitialLayout(RenderAttachmentLayout::Undefined)
                                                .setFinalLayout(RenderAttachmentLayout::Present))
                             .addPass(RenderSubgraphPassInfo().addColorAttachmentIdx(0).setOperatorInfo(
                                 RenderOperatorInfo(ImGuiRenderOperator::name(), std::move(uiParams))
                             ))
                     );

    auto renderContext = engine.resources().get<RenderGraphContext>();
    renderContext->setRenderGraph(graph);

    REQUIRE_NOTHROW([&]() { engine.run(); }());
}
