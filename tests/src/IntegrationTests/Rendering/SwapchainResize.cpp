#include <catch2/catch_all.hpp>

#include <Penrose/Engine.hpp>

#include "src/ECS/TestCountdownSystem.hpp"
#include "src/ECS/TestSurfaceResizeSystem.hpp"

using namespace Penrose;

TEST_CASE("SwapchainResize", "[engine-int-test]") {
    Engine engine;

    engine.resources().add<TestCountdownSystem, ResourceGroup::ECSSystem>()
            .implements<Initializable>()
            .implements<System>()
            .done();

    engine.resources().add<TestSwapchainResizeSystem, ResourceGroup::ECSSystem>()
            .implements<Initializable>()
            .implements<System>()
            .done();

    REQUIRE_NOTHROW([&]() { engine.run(); }());
}
