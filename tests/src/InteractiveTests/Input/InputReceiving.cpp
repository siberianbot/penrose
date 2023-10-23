#include <catch2/catch_all.hpp>

#include <Penrose/Engine.hpp>
#include <Penrose/Common/Log.hpp>
#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Events/InputEvent.hpp>
#include <Penrose/Resources/Initializable.hpp>
#include <Penrose/Resources/ResourceSet.hpp>

using namespace Penrose;

TEST_CASE("InputReceiving", "[engine-interactive-test]") {

    constexpr static std::string_view TEST_LOGIC_SYSTEM_TAG = "InputReceiving";

    class TestLogicSystem : public Resource<TestLogicSystem>,
                            public Initializable,
                            public System {
    public:
        explicit TestLogicSystem(ResourceSet *resources)
                : _eventQueue(resources->get<EventQueue>()),
                  _log(resources->get<Log>()) {
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
        ResourceProxy<EventQueue> _eventQueue;
        ResourceProxy<Log> _log;

        EventQueue::HandlerIdx _eventHandlerIdx = -1;
    };

    Engine engine;

    engine.resources().add<TestLogicSystem>()
            .implements<Initializable>()
            .implements<System>()
            .done();

    REQUIRE_NOTHROW([&]() { engine.run(); }());
}
