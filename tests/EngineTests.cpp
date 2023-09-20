#include <catch2/catch.hpp>

#include <Penrose/Core/Engine.hpp>
#include <Penrose/ECS/System.hpp>
#include <Penrose/Events/EventQueue.hpp>
#include <Penrose/Resources/Resource.hpp>

using namespace Penrose;

TEST_CASE("Start engine and immediately exit by EngineDestroyRequested event", "[engine][event-queue]") {
    Engine engine;

    engine.resources().get<EventQueue>()->push(makeEvent(EventType::EngineDestroyRequested));

    engine.run();
}

TEST_CASE("Start engine and exit by EngineDestroyRequested event after short period of time",
          "[engine][event-queue][ecs][ecs-system]") {
    class CountdownSystem : public Resource, public System {
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

        [[nodiscard]] std::string getName() const override { return "CountdownSystem"; }

    private:
        EventQueue *_eventQueue;
        float _passed = 0;
    };

    Engine engine;
    engine.resources().add<CountdownSystem, System>();

    engine.run();
}
