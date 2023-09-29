#include <catch2/catch.hpp>

#include <Penrose/Engine.hpp>

#include "src/ECS/TestCountdownSystem.hpp"
#include "src/ECS/TestSurfaceResizeSystem.hpp"

using namespace Penrose;

TEST_CASE("SwapchainResize", "[engine-int-test]") {
    Engine engine;

    engine.resources().add<TestCountdownSystem, System>();
    engine.resources().add<TestSwapchainResizeSystem, System>();

    REQUIRE_NOTHROW([&]() { engine.run(); }());
}
