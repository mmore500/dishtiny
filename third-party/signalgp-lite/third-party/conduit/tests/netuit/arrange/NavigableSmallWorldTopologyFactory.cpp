#include <fstream>

#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/arrange/NavigableSmallWorldTopologyFactory.hpp"

#include "NetworkXTester.hpp"

TEST_CASE("Test NavigableSmallWorldTopologyFactory", "[nproc:1]") {
  REQUIRE(test_all_adj(netuit::NavigableSmallWorldTopologyFactory{}));
}
