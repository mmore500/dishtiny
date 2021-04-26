#include <fstream>

#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/arrange/ToroidalTopologyFactory.hpp"

#include "NetworkXTester.hpp"

TEST_CASE("Test ToroidalTopologyFactory", "[nproc:1]") {
  REQUIRE(test_all_adj(netuit::ToroidalTopologyFactory{}));
}
