#pragma once
#ifndef NETUIT_ARRANGE_SMALLWORLDGRIDTOPOLOGYFACTORY_HPP_INCLUDE
#define NETUIT_ARRANGE_SMALLWORLDGRIDTOPOLOGYFACTORY_HPP_INCLUDE

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
 * @param p The probability of each node engaging in a long distance connection.
 * @param dim Dimension of grid
 */
inline Topology make_small_world_grid_topology(
  const size_t n,
  const double p=1.0,
  const size_t dim=1
) {

  const auto tmpfile = uitsl::make_temp_filepath();

  const std::string command_template = std::string{"python3 -c \""}
    + uitsl::unindent_raw_string_literal( R"(
      import networkx as nx
      import random

      random.seed(1)

      G = nx.grid_graph(
        [%u for __ in range(%u)], periodic=True
      ).to_directed()

      nodes = list(G.nodes())
      random.shuffle( nodes )
      for a, b in zip(nodes[0::2], nodes[1::2]):
        if (random.random() < %f):
          G.add_edge(a, b)
          G.add_edge(b, a)

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
    command_template, n, dim, p, tmpfile.c_str()
  );

  uitsl::err_verify( std::system( command.c_str() ) );

  return netuit::make_adjacency_file_topology( tmpfile );

}

template<typename P=std::ratio<1>>
struct SmallWorldGridTopologyFactory {

  Topology operator()(const size_t cardinality) const {

    return make_small_world_grid_topology(
      cardinality, static_cast<double>(P::num) / P::den, 1
    );

  }

  netuit::Topology operator()(const emp::vector<size_t>& cardinality) const {
    emp_assert(
      std::set<size_t>(
        std::begin(cardinality), std::end(cardinality)
      ).size() == 1
    );
    return make_small_world_grid_topology(
      cardinality.front(),
      static_cast<double>(P::num) / P::den,
      cardinality.size()
    );
  }

  static std::string GetName() { return "Small World Grid Topology"; }

  static std::string GetSlug() { return "small_world_grid"; }

};

} // namespace netuit

#endif // #ifndef NETUIT_ARRANGE_SMALLWORLDGRIDTOPOLOGYFACTORY_HPP_INCLUDE
