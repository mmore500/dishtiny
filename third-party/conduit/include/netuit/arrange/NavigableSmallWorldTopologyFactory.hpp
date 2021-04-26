#pragma once
#ifndef NETUIT_ARRANGE_NAVIGABLESMALLWORLDTOPOLOGYFACTORY_HPP_INCLUDE
#define NETUIT_ARRANGE_NAVIGABLESMALLWORLDTOPOLOGYFACTORY_HPP_INCLUDE

#include <cstdlib>
#include <fstream>
#include <ratio>
#include <set>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../uitsl/debug/err_verify.hpp"
#include "../../uitsl/fetch/make_temp_filepath.hpp"
#include "../../uitsl/utility/unindent_raw_string_literal.hpp"

#include "../topology/TopoEdge.hpp"
#include "../topology/Topology.hpp"
#include "../topology/TopoNode.hpp"

#include "AdjacencyFileTopologyFactory.hpp"

namespace netuit {

/*
 * @param n The length of one side of the lattice; the number of nodes in the graph is therefore $n^2$.
 * @param p The diameter of short range connections.
 *  Each node is joined with every other node within this lattice distance.
 * @param q The number of long-range connections for each node.
 * @param r Exponent for decaying probability of connections.
 *   The probability of connecting to a node at lattice distance $d$ is $d^{-r}$.
 * @param dim Dimension of grid
 */
inline Topology make_navigable_small_world_topology(
  const size_t n,
  const size_t p=1,
  const size_t q=1,
  const double r=2,
  const size_t dim=1
) {

  const auto tmpfile = uitsl::make_temp_filepath();

  const std::string command_template = std::string{"python3 -c \""}
    + uitsl::unindent_raw_string_literal( R"(
      import networkx as nx

      G = nx.generators.geometric.navigable_small_world_graph(
        %u, p=%u, q=%u, r=%f, dim=%u, seed=1
      )

      # relabel nodes to numeric indices from previous coordinate labels
      H = nx.relabel_nodes(
        G,
        { label : idx for idx, label in enumerate(G.nodes()) },
      )

      with open('%s', 'w') as file:
          for line in nx.generate_adjlist(H):
              file.write(line + '\n')
      )" ) + "\"";

  const std::string command = emp::format_string(
    command_template, n, p, q, r, dim, tmpfile.c_str()
  );

  uitsl::err_verify( std::system( command.c_str() ) );

  return netuit::make_adjacency_file_topology( tmpfile );

}

template<size_t P=1, size_t Q=1, typename R=std::ratio<2>>
struct NavigableSmallWorldTopologyFactory {

  Topology operator()(const size_t cardinality) const {

    return make_navigable_small_world_topology(
      cardinality, P, Q, static_cast<double>( R::num ) / R::den, 1
    );

  }

  netuit::Topology operator()(const emp::vector<size_t>& cardinality) const {
    emp_assert(
      std::set<size_t>(
        std::begin(cardinality), std::end(cardinality)
      ).size() == 1
    );
    return make_navigable_small_world_topology(
      cardinality.front(),
      P, Q, static_cast<double>( R::num ) / R::den,
      cardinality.size()
    );
  }

  static std::string GetName() { return "Navigable Small World Topology"; }

  static std::string GetSlug() { return "navigable_small_world"; }

};

} // namespace netuit

#endif // #ifndef NETUIT_ARRANGE_NAVIGABLESMALLWORLDTOPOLOGYFACTORY_HPP_INCLUDE
