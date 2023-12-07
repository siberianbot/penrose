#include <catch2/catch_all.hpp>

#include <string>

#include "../src/Common/OrderedQueue.hpp"

using namespace Penrose;

TEST_CASE("Common / OrdererQueue<int>", "[Common][OrderedQueue]") {

    OrderedQueue<int> queue;

    queue.push(42);
    queue.push(21);
    queue.push(63);
    queue.push(32);
    queue.push(11);

    REQUIRE(queue.front() == 11);

    queue.push(2);
    queue.push(2);

    const auto expectedSequence = {2, 2, 11, 21, 32, 42, 63};

    for (const int expectedItem: expectedSequence) {
        REQUIRE(queue.front() == expectedItem);

        queue.pop();
    }

    REQUIRE(queue.empty());
}

TEST_CASE("Common / OrdererQueue<Custom, CustomEqualTo, CustomLess>", "[Common][OrderedQueue]") {

    struct Custom {
        int order;
        std::string value;
    };

    struct CustomEqualTo {
        bool operator()(const Custom &l, const Custom &r) const { return l.order == r.order; }
    };

    struct CustomLess {
        bool operator()(const Custom &l, const Custom &r) const { return l.order < r.order; }
    };

    OrderedQueue<Custom, CustomEqualTo, CustomLess> queue;

    queue.push({42, "f"});
    queue.push({21, "d"});
    queue.push({63, "g"});
    queue.push({32, "e"});
    queue.push({11, "c"});
    queue.push({2, "a"});
    queue.push({2, "b"});

    const auto expectedSequence = {"a", "b", "c", "d", "e", "f", "g"};

    for (const std::string expectedItem: expectedSequence) {
        REQUIRE(queue.front().value == expectedItem);

        queue.pop();
    }

    REQUIRE(queue.empty());
}
