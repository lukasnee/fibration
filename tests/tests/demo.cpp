#include "catch.hpp"

#include <vector>

TEST_CASE("demo")
{
    SECTION("1")
    {
        std::vector<int> inputs = {1, 2, 3, 4};
        REQUIRE(inputs[3] == 4);
    }
}
