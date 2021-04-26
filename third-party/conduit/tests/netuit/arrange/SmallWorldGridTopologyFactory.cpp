#include <fstream>

#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/arrange/SmallWorldGridTopologyFactory.hpp"

#include "NetworkXTester.hpp"

TEST_CASE("Test SmallWorldGridTopologyFactory", "[nproc:1]") {
  REQUIRE(test_all_adj(netuit::SmallWorldGridTopologyFactory{}));
}
