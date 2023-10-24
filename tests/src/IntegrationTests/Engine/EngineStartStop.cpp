#include <catch2/catch_all.hpp>

#include <Penrose/Engine.hpp>
#include <Penrose/Events/EngineEvents.hpp>

using namespace Penrose;

TEST_CASE("EngineStartStop", "[engine-int-test]") {
    Engine engine;

    engine.resources().get<EngineEventQueue>()->push<EngineDestroyRequestedEvent>();

    REQUIRE_NOTHROW([&]() { engine.run(); }());
}
