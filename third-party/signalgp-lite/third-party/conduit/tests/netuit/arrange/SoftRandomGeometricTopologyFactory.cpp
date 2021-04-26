#include <fstream>

#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/arrange/SoftRandomGeometricTopologyFactory.hpp"

#include "NetworkXTester.hpp"

TEST_CASE("Test SoftRandomGeometricTopologyFactory", "[nproc:1]") {
  REQUIRE(test_all_adj(netuit::SoftRandomGeometricTopologyFactory{}));
}
