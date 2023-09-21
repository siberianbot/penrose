#include <catch2/catch.hpp>

#include <Penrose/Core/Engine.hpp>
#include <Penrose/Events/EventQueue.hpp>

using namespace Penrose;

TEST_CASE("EngineStartStop", "[engine-int-test]") {
    Engine engine;

    engine.resources().get<EventQueue>()->push(makeEvent(EventType::EngineDestroyRequested));

    REQUIRE_NOTHROW([&]() { engine.run(); }());
}
