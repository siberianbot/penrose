#include <catch2/catch.hpp>

#include <Penrose/Core/Engine.hpp>
#include <Penrose/ECS/ECSManager.hpp>
#include <Penrose/Events/EventQueue.hpp>

using namespace Penrose;

TEST_CASE("Start engine and immediately exit by EngineDestroyRequested event", "[engine][event-queue]") {
    Engine engine;

    engine.resources().get<EventQueue>()->push(makeEvent(EventType::EngineDestroyRequested));

    engine.run();
}

TEST_CASE("Start engine and exit by EngineDestroyRequested event after short period of time", "[engine][event-queue]") {
    class CountdownSystem : public System {
    public:
        explicit CountdownSystem(ResourceSet *resources)
                : _eventQueue(resources->get<EventQueue>()) {
            //
        }

        ~CountdownSystem() override = default;

        void update(float delta) override {
            this->_passed += delta;

            if (this->_passed > 1.0f) {
                this->_eventQueue->push(makeEvent(EventType::EngineDestroyRequested));
            }
        }

        [[nodiscard]] static std::string_view name() { return "CountdownSystem"; }

    private:
        EventQueue *_eventQueue;
        float _passed = 0;
    };

    Engine engine;

    auto ecsManager = engine.resources().get<ECSManager>();
    ecsManager->registerSystem<CountdownSystem>();
    ecsManager->enableSystem<CountdownSystem>();

    engine.run();
}
