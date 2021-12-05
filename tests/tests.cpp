

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_NO_POSIX_SIGNALS

#include <vector>

#include "catch.hpp"

TEST_CASE("asd") {

  SECTION("goodgoodnotbad") {
    std::vector<int> inputs = {1, 2, 3, 4};
    REQUIRE(inputs[3] == 4);
  }
}
