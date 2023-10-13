#include <catch2/catch_all.hpp>

#include <Penrose/Math/NumericFuncs.hpp>

using namespace Penrose;

TEST_CASE("NumericFuncs_Cycle", "[engine-unit-test]") {

    CHECK(cycle(-42, 0, 42.0) == 0);
    CHECK(cycle<float>(1, 0, 0.5f) == 0.5f);
}
