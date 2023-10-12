#include <catch2/catch_all.hpp>

#include <Penrose/Engine.hpp>

#include "src/ECS/TestCountdownSystem.hpp"

using namespace Penrose;

TEST_CASE("EngineStartWaitStop", "[engine-int-test]") {
    Engine engine;

    auto testCountdownSystem = engine.resources().add<TestCountdownSystem, System>();
    testCountdownSystem->setTestTimeout(1.0f);

    REQUIRE_NOTHROW([&]() { engine.run(); }());
}
