#include <catch2/catch_all.hpp>

#include "../src/Common/BitSet.hpp"

using namespace Penrose;

TEST_CASE("Common / BitSet", "[Common][BitSet]") {

    constexpr auto baseSize = 64;

    BitSet bitset;

    REQUIRE(bitset.size() > 0);

    bitset.set(5, true);

    REQUIRE(bitset.at(5));
    REQUIRE(bitset.any());

    REQUIRE_FALSE(bitset.all());

    for (std::size_t idx = 0; idx < bitset.size(); idx++) {
        bitset.set(idx, true);
    }

    REQUIRE(bitset.all());

    bitset.clear();

    REQUIRE_FALSE(bitset.all());

    bitset.set(31, true);
    bitset.set(33, true);
    bitset.resize(baseSize / 2);

    REQUIRE(bitset.size() == (baseSize / 2));
    REQUIRE(bitset.at(31));

    bitset.resize(baseSize);

    REQUIRE(bitset.size() == baseSize);
    REQUIRE(bitset.at(31));
    REQUIRE_FALSE(bitset.at(33));

    bitset.resize(baseSize * 4);
    bitset.set(baseSize * 2, true);

    REQUIRE(bitset.size() == baseSize * 4);
    REQUIRE(bitset.at(baseSize * 2));

    bitset.resize(baseSize);

    REQUIRE(bitset.size() == baseSize);

    bitset.resize(baseSize * 4);

    REQUIRE(bitset.size() == baseSize * 4);
    REQUIRE_FALSE(bitset.at(baseSize * 2));
}
