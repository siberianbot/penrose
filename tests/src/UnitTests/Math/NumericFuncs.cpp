#include <catch2/catch_all.hpp>

#include <Penrose/Math/NumericFuncs.hpp>

using namespace Penrose;

TEST_CASE("NumericFuncs_Cycle", "[engine-unit-test]") {

    CHECK(cycle(-42, 0, 42.0) == 0);
    CHECK(cycle<float>(1, 0, 0.5f) == 0.5f);
}

TEST_CASE("NumericFuncs_Max", "[engine-unit-test]") {

    CHECK(max(-42, 0, 42) == 42);
    CHECK(max(2.0f, 1) == 2.0f);
    CHECK(max(0.0f, 1, static_cast<std::uint8_t>(2), 3.) == 3.);
}
