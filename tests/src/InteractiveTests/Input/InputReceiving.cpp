#include <catch2/catch_all.hpp>

#include <Penrose/Engine.hpp>
#include <Penrose/Common/Log.hpp>
#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Events/InputEvents.hpp>
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

    engine.resources().add<TestLogicSystem>()
            .implements<Initializable>()
            .implements<System>()
            .done();

    REQUIRE_NOTHROW([&]() { engine.run(); }());
}
