#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/arrange/ProConTopologyFactory.hpp"

#include "NetworkXTester.hpp"


TEST_CASE("Test Procon file output", "[nproc:1]") {
  REQUIRE(test_all_adj(netuit::ProConTopologyFactory{}));
}
