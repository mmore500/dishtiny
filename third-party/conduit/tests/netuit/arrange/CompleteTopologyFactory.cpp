#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/arrange/CompleteTopologyFactory.hpp"

#include "NetworkXTester.hpp"

TEST_CASE("Test file output", "[nproc:1]") {
  // TODO: implement matchers to move this require further in?
  // https://github.com/catchorg/Catch2/blob/master/docs/matchers.md
  REQUIRE(test_all_adj(netuit::CompleteTopologyFactory{}));
}
