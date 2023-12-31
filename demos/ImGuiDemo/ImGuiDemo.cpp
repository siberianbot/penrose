#include <Penrose/Common/Log.hpp>
#include <Penrose/Engine.hpp>
#include <Penrose/Events/EngineEvents.hpp>
#include <Penrose/Events/InputEvents.hpp>
#include <Penrose/Rendering/RenderManager.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>
#include <Penrose/UI/UIManager.hpp>
#include <Penrose/UI/Value.hpp>

#include <Penrose/Builtin/ImGui/Rendering/ImGuiRenderer.hpp>

using namespace Penrose;

class DemoUIContext final: public Resource<DemoUIContext>,
                           public Initializable {
public:
    explicit DemoUIContext(const ResourceSet *resources)
        : _uiManager(resources->get<UIManager>()),
          _engineEventQueue(resources->get<EngineEventQueue>()) {
        //
    }

    ~DemoUIContext() override = default;

    void init() override {
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
                            "close", [this]() { this->_engineEventQueue->push<EngineDestroyRequestEvent>(); }
                        )
                        .property<StringValue>("text", "Same text in label and input")
                        .property<ActionValue>("button_action", [buttonPressed]() { buttonPressed->setValue(true); })
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
                                    "value", fmt::format("Item {}", listItems->getItems().size() + 1)
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
                            ListValue<StringValue>().push("Item 1").push("Item 2").push("Item 3").push("One more thing")
                        )
                        .property<IntegerValue>("select_selected", -1)
                )
                .property<BooleanValue>("root_opened", true)
        );

        this->_uiManager->createContext("TestUI");
        this->_uiManager->addLayoutToContext("TestUI", "layouts/root", std::move(viewModel));
    }

    void destroy() override { this->_uiManager->destroyContext("TestUI"); }

private:
    ResourceProxy<UIManager> _uiManager;
    ResourceProxy<EngineEventQueue> _engineEventQueue;
};

class TestLogic final: public Resource<TestLogic>,
                       public Initializable {
public:
    inline constexpr static std::string_view TEST_LOGIC_TAG = "InputReceiving";

    explicit TestLogic(const ResourceSet *resources)
        : _eventQueue(resources->get<InputEventQueue>()),
          _log(resources->get<Log>()) {
        //
    }

    ~TestLogic() override = default;

    void init() override {
        this->_eventQueue->addHandler<KeyStateUpdatedEvent>([this](const KeyStateUpdatedEvent *event) {
            this->_log->writeDebug(
                TEST_LOGIC_TAG, "Key state updated - key {:#x}, state {:#x}", static_cast<std::uint32_t>(event->key),
                static_cast<std::uint32_t>(event->state)
            );
        });

        this->_eventQueue->addHandler<MouseMovementEvent>([this](const MouseMovementEvent *event) {
            auto [x, y] = event->position;
            auto [dx, dy] = event->movement;

            this->_log->writeDebug(TEST_LOGIC_TAG, "Mouse moved - x = {}, y = {}, dx = {}, dy = {}", x, y, dx, dy);
        });

        this->_eventQueue->addHandler<MouseScrollEvent>([this](const MouseScrollEvent *event) {
            auto [dx, dy] = event->scroll;

            this->_log->writeDebug(TEST_LOGIC_TAG, "Scroll - dx = {}, dy = {}", dx, dy);
        });
    }

    void destroy() override {
        // nothing to do
    }

private:
    ResourceProxy<InputEventQueue> _eventQueue;
    ResourceProxy<Log> _log;
};

int main() {

    Engine engine;

    engine.resources().add<DemoUIContext>().implements<Initializable>().done();
    engine.resources().add<TestLogic>().implements<Initializable>().done();

    auto assetManager = engine.resources().get<AssetManager>();
    assetManager->addDir("data");

    auto renderManager = engine.resources().get<RenderManager>();
    renderManager->addRenderer<ImGuiRenderer>();
    renderManager->setExecutionInfo(RenderExecutionInfo {
        .renderers =
            {
                        RendererExecutionInfo(ImGuiRenderer::name(), Params().add("Context", "TestUI")),
                        }
    });

    engine.run();
}
