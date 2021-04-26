
#include "Catch/single_include/catch2/catch.hpp"
#include "Empirical/include/emp/base/vector.hpp"

#include "netuit/arrange/CompleteTopologyFactory.hpp"
#include "netuit/arrange/DyadicTopologyFactory.hpp"
#include "netuit/arrange/EmptyTopologyFactory.hpp"
#include "netuit/arrange/ToroidalTopologyFactory.hpp"
#include "netuit/topology/Topology.hpp"

TEST_CASE("Test Topology", "[nproc:1]") {

  // TODO flesh out stub test
  netuit::Topology{};

}

TEST_CASE("Test AsCSR Null Topology", "[nproc:1]") {

  netuit::Topology topology{};
  const auto [x_adj, adjacency] = topology.AsCSR();

  REQUIRE( x_adj == emp::vector<int>{} );
  REQUIRE( adjacency == emp::vector<int>{} );

}

TEST_CASE("Test AsCSR CompleteTopologyFactory", "[nproc:1]") {

  netuit::Topology topology = netuit::make_complete_topology( 3 );
  const auto [x_adj, adjacency] = topology.AsCSR();

  REQUIRE( x_adj == emp::vector<int>{ 0, 2, 4, 6 } );
  REQUIRE( adjacency == emp::vector<int>{ 1, 2, 0, 2, 0, 1 } );


}

TEST_CASE("Test AsCSR DyadicTopologyFactory", "[nproc:1]") {

  netuit::Topology topology = netuit::make_dyadic_topology( 3 );
  const auto [x_adj, adjacency] = topology.AsCSR();

  REQUIRE( x_adj == emp::vector<int>{ 0, 1, 2, 3 } );
  REQUIRE( adjacency == emp::vector<int>{ 1, 0, 2 } );


}

TEST_CASE("Test AsCSR Empty Topology", "[nproc:1]") {

  netuit::Topology topology = netuit::make_empty_topology( 3 );
  const auto [x_adj, adjacency] = topology.AsCSR();

  REQUIRE( x_adj == emp::vector<int>{ 0, 0, 0, 0 } );
  REQUIRE( adjacency == emp::vector<int>{} );

}

TEST_CASE("Test AsCSR ToroidalTopologyFactory", "[nproc:1]") {

  netuit::Topology topology = netuit::make_toroidal_topology( {3, 3} );
  const auto [x_adj, adjacency] = topology.AsCSR();

  /*
   * 0 1 2
   * 3 4 5
   * 6 7 8
   */

  REQUIRE( x_adj == emp::vector<int>{ 0, 4, 8, 12, 16, 20, 24, 28, 32, 36 } );
  REQUIRE( adjacency == emp::vector<int>{
    6, 2, 1, 3, // 0's neighbors
    7, 0, 2, 4, // 1's neighbors
    8, 1, 0, 5, // 2's neighbors
    0, 5, 4, 6, // 3's neighbors
    1, 3, 5, 7, // 4's neighbors
    2, 4, 3, 8, // 5's neighbors
    3, 8, 7, 0, // 6's neighbors
    4, 6, 8, 1, // 7's neighbors
    5, 7, 6, 2, // 8's neighbors
  } );


}

TEST_CASE("Test trivial Subtopology, ToroidalTopologyFactory", "[nproc:1]") {

  netuit::Topology topology = netuit::make_toroidal_topology( {3, 3} );
  netuit::Topology subtopo = topology.GetSubTopology(
    {0, 1, 2, 3, 4, 5, 6, 7, 8}
  );

  auto [x_adj, adjacency] = subtopo.AsCSR();

  /*
   * 0 1 2
   * 3 4 5
   * 6 7 8
   */

  REQUIRE( x_adj == emp::vector<int>{ 0, 4, 8, 12, 16, 20, 24, 28, 32, 36 } );
  REQUIRE( adjacency == emp::vector<int>{
    3, 1, 2, 6, // 0's neighbors
    4, 2, 0, 7, // 1's neighbors
    5, 0, 1, 8, // 2's neighbors
    6, 4, 5, 0, // 3's neighbors
    7, 5, 3, 1, // 4's neighbors
    8, 3, 4, 2, // 5's neighbors
    0, 7, 8, 3, // 6's neighbors
    1, 8, 6, 4, // 7's neighbors
    2, 6, 7, 5, // 8's neighbors
  } );

}

TEST_CASE("Test Subtopology, ToroidalTopologyFactory", "[nproc:1]") {

  netuit::Topology topology = netuit::make_toroidal_topology( {3, 3} );
  netuit::Topology subtopo = topology.GetSubTopology({0, 1, 2, 3, 4, 5});

  auto [x_adj, adjacency] = subtopo.AsCSR();

  REQUIRE( x_adj == emp::vector<int>{ 0, 3, 6, 9, 12, 15, 18 } );
  REQUIRE( adjacency == emp::vector<int>{
    3, 1, 2, // 0's neighbors
    4, 2, 0, // 1's neighbors
    5, 0, 1, // 2's neighbors
    4, 5, 0, // 3's neighbors
    5, 3, 1, // 4's neighbors
    3, 4, 2, // 5's neighbors
  } );
}

TEST_CASE("Test Topology istream ctor", "[nproc:1]") {
  std::stringstream iss(
    "0 1\n"
    "1 2\n"
    "2 3\n"
    "3 0\n"
    "4\n"
  );

  netuit::Topology topology(dynamic_cast<std::istream&>(iss));

  std::stringstream oss;
  topology.PrintAdjacencyList(oss);

  REQUIRE(iss.str() == oss.str());

}
