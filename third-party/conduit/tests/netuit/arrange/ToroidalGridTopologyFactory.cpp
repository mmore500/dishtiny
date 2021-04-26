#include <fstream>

#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/arrange/ToroidalGridTopologyFactory.hpp"

#include "NetworkXTester.hpp"

TEST_CASE("Test ToroidalGridTopologyFactory", "[nproc:1]") {
  REQUIRE(test_all_adj(netuit::ToroidalGridTopologyFactory{}));
}
