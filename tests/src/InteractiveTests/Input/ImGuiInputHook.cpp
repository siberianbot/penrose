#include <catch2/catch_all.hpp>

#include <Penrose/Engine.hpp>
#include <Penrose/Common/Log.hpp>
#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Events/InputEvent.hpp>
#include <Penrose/Rendering/RenderGraphContext.hpp>
#include <Penrose/Rendering/RenderGraphInfo.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/Lazy.hpp>
#include <Penrose/Resources/Resource.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

#include <Penrose/Builtin/Debug/Rendering/DebugUIDrawRenderOperator.hpp>
#include <Penrose/Builtin/Debug/UI/UIContext.hpp>
#include <Penrose/Builtin/Debug/UI/Widgets.hpp>

using namespace Penrose;

TEST_CASE("ImGuiInputHook", "[engine-interactive-test]") {

    constexpr static std::string_view TEST_LOGIC_SYSTEM_TAG = "InputReceiving";

    class TestLogicSystem : public Resource, public Initializable, public System {
    public:
        explicit TestLogicSystem(ResourceSet *resources)
                : _eventQueue(resources->getLazy<EventQueue>()),
                  _log(resources->getLazy<Log>()) {
            //
        }

        ~TestLogicSystem() override = default;

        void init() override {
            this->_eventHandlerIdx = this->_eventQueue->addHandler<EventType::InputEvent, InputEventArgs>(
                    [this](const InputEvent *event) {
                        switch (event->getArgs().type) {
                            case InputEventType::KeyStateUpdated: {
                                auto [key, state] = event->getArgs().keyState;

                                this->_log->writeDebug(TEST_LOGIC_SYSTEM_TAG,
                                                       "Key state updated - key {:#x}, state {:#x}",
                                                       static_cast<std::uint32_t>(key),
                                                       static_cast<std::uint32_t>(state));
                                break;
                            }

                            case InputEventType::MouseMoved: {
                                auto [x, y] = event->getArgs().mousePos;
                                auto [dx, dy] = event->getArgs().mousePosDelta;

                                this->_log->writeDebug(TEST_LOGIC_SYSTEM_TAG,
                                                       "Mouse moved - x = {}, y = {}, dx = {}, dy = {}", x, y, dx, dy);
                                break;
                            }

                            default:
                                auto [dx, dy] = event->getArgs().scroll;

                                this->_log->writeDebug(TEST_LOGIC_SYSTEM_TAG,
                                                       "Scroll - dx = {}, dy = {}", dx, dy);
                                break;
                        }
                    });
        }

        void update(float) override { /* nothing to do */ }

        void destroy() override {
            this->_eventQueue->removeHandler(this->_eventHandlerIdx);
        }

        [[nodiscard]] std::string getName() const override { return "TestLogic"; }

    private:
        Lazy<EventQueue> _eventQueue;
        Lazy<Log> _log;

        EventQueue::HandlerIdx _eventHandlerIdx = -1;
    };

    Engine engine;

    engine.resources().add<TestLogicSystem, System>();

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
