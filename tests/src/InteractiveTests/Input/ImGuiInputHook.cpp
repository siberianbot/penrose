#include <catch2/catch_all.hpp>

#include <Penrose/Engine.hpp>
#include <Penrose/Common/Log.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Events/InputEvents.hpp>
#include <Penrose/Rendering/RenderGraphContext.hpp>
#include <Penrose/Rendering/RenderGraphInfo.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include <Penrose/Builtin/Debug/Rendering/DebugUIDrawRenderOperator.hpp>
#include <Penrose/Builtin/Debug/UI/UIContext.hpp>
#include <Penrose/Builtin/Debug/UI/Widgets.hpp>

using namespace Penrose;

TEST_CASE("ImGuiInputHook", "[engine-interactive-test]") {

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

    engine.resources().add<TestLogicSystem, ResourceGroup::ECSSystem>()
            .implements<Initializable>()
            .implements<System>()
            .done();

    auto buttonLabel = std::make_shared<Label>("Not clicked yet");

    auto window = std::shared_ptr<Window>( // NOLINT(modernize-make-shared)
            new Window("Debug UI Window", {
                    std::make_shared<Label>("It's a label"),
                    std::make_shared<TextInput>("It's a text input"),
                    buttonLabel,
                    std::make_shared<Button>("Click me", [&]() {
                        buttonLabel->setText("Clicked!");
                    }),
                    std::shared_ptr<DropDown>(new DropDown( // NOLINT(modernize-make-shared)
                            "Select inside of me", {
                                    {1, "Item 1"},
                                    {2, "Item 2"},
                                    {3, "Item 3"},
                                    {4, "Item 4"}
                            })),
                    std::shared_ptr<ListBox>(new ListBox( // NOLINT(modernize-make-shared)
                            "Select inside of me", {
                                    {0, "Item 0"},
                                    {1, "Item 1"},
                                    {2, "Item 2"},
                                    {3, "Item 3"},
                                    {4, "Item 4"}
                            })),
                    std::shared_ptr<Container>( // NOLINT(modernize-make-shared)
                            new Container({
                                                  std::make_shared<Label>("inside container")
                                          })
                    )
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
