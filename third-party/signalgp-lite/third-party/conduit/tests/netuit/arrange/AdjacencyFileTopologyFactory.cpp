#include <algorithm>
#include <iterator>

#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/arrange/AdjacencyFileTopologyFactory.hpp"

TEST_CASE("Test file input", "[nproc:1]") {
  // TODO: implement matchers to move this require further in?
  // https://github.com/catchorg/Catch2/blob/master/docs/matchers.md

  const std::string filename = "assets/dim0=15+name=complete+ndims=1+ext=.adj";
  netuit::Topology topology = netuit::AdjacencyFileTopologyFactory{}(filename);

  std::stringstream toposs;
  topology.PrintAdjacencyList(toposs);

  // open file ourselves
  std::fstream file(filename);
  emp_assert(file);

  // cope file into stringstream
  std::stringstream filess;

  std::copy(
    std::istreambuf_iterator<char>(file),
    std::istreambuf_iterator<char>(),
    std::ostreambuf_iterator<char>(filess)
  );

  REQUIRE(toposs.str() == filess.str());
}
