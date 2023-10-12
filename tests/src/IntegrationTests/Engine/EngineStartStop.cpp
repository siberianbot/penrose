#include <catch2/catch_all.hpp>

#include <Penrose/Engine.hpp>
#include <Penrose/Events/EngineEvent.hpp>
#include <Penrose/Events/EventQueue.hpp>

using namespace Penrose;

TEST_CASE("EngineStartStop", "[engine-int-test]") {
    Engine engine;

    auto data = EngineEventArgs{
            .type= EngineEventType::DestroyRequested
    };

    engine.resources().get<EventQueue>()->pushEvent<Penrose::EventType::EngineEvent>(data);

    REQUIRE_NOTHROW([&]() { engine.run(); }());
}
