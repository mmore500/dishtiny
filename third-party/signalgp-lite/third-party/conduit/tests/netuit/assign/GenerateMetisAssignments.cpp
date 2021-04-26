#include <mpi.h>

#include "Catch/single_include/catch2/catch.hpp"

#include "netuit/arrange/CompleteTopologyFactory.hpp"
#include "netuit/arrange/DyadicTopologyFactory.hpp"
#include "netuit/arrange/ToroidalTopologyFactory.hpp"
#include "netuit/assign/GenerateMetisAssignments.hpp"
#include "netuit/topology/Topology.hpp"

TEST_CASE("Test PartitionMetis, complete topology") {
  netuit::Topology topo1 = netuit::make_complete_topology( 1 );
  netuit::PartitionMetis(1, topo1);

  netuit::Topology topo16 = netuit::make_complete_topology( 16 );
  netuit::PartitionMetis(1, topo16);
  netuit::PartitionMetis(2, topo16);

  netuit::Topology topo17 = netuit::make_complete_topology( 17 );
  netuit::PartitionMetis(1, topo17);
  netuit::PartitionMetis(2, topo17);
}

TEST_CASE("Test GenerateMetisAssignments, complete topology") {
  netuit::Topology topo1 = netuit::make_complete_topology( 1 );
  netuit::GenerateMetisAssignments(1, 1, topo1);

  netuit::Topology topo16 = netuit::make_complete_topology( 16 );
  netuit::GenerateMetisAssignments(1, 1, topo16);
  netuit::GenerateMetisAssignments(2, 2, topo16);

  netuit::Topology topo17 = netuit::make_complete_topology( 17 );
  netuit::GenerateMetisAssignments(1, 1, topo17);
  netuit::GenerateMetisAssignments(2, 2, topo17);
}

TEST_CASE("Test GenerateMetisAssignments, dyadic topology") {
  netuit::Topology topo1 = netuit::make_dyadic_topology( 1 );
  netuit::GenerateMetisAssignments(1, 1, topo1);

  netuit::Topology topo16 = netuit::make_dyadic_topology( 16 );
  netuit::GenerateMetisAssignments(1, 1, topo16);
  netuit::GenerateMetisAssignments(2, 2, topo16);

  netuit::Topology topo17 = netuit::make_dyadic_topology( 17 );
  netuit::GenerateMetisAssignments(1, 1, topo17);
  netuit::GenerateMetisAssignments(2, 2, topo17);
}

TEST_CASE("Test GenerateMetisAssignments, toroidal topology") {
  netuit::Topology topo1 = netuit::make_toroidal_topology( {1} );
  netuit::GenerateMetisAssignments(1, 1, topo1);

  netuit::Topology topo16 = netuit::make_toroidal_topology( {16, 16} );
  netuit::GenerateMetisAssignments(1, 1, topo16);
  netuit::GenerateMetisAssignments(2, 2, topo16);

  netuit::Topology topo17 = netuit::make_toroidal_topology( {17, 17, 17} );
  netuit::GenerateMetisAssignments(1, 1, topo17);
  netuit::GenerateMetisAssignments(2, 2, topo17);
}
