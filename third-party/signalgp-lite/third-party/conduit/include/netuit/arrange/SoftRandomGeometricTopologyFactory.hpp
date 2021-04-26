#pragma once
#ifndef NETUIT_ARRANGE_SOFTRANDOMGEOMETRICTOPOLOGYFACTORY_HPP_INCLUDE
#define NETUIT_ARRANGE_SOFTRANDOMGEOMETRICTOPOLOGYFACTORY_HPP_INCLUDE

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
* @param n Number of nodes
* @param radius Distance threshold value
* @param dim Dimension of graph
 */
inline Topology make_soft_random_geometric_topology(
  const size_t n,
  const double radius=0.1,
  const size_t dim=2
) {

  const auto tmpfile = uitsl::make_temp_filepath();

  const std::string command_template = std::string{"python3 -c \""}
    + uitsl::unindent_raw_string_literal( R"(
      import networkx as nx
      import numpy as np

      G = nx.generators.geometric.soft_random_geometric_graph(
        %u, %f, dim=%u, seed=1
      )

      print('mean degree', np.mean([val for (node, val) in G.degree()]))

      with open('%s', 'w') as file:
          for line in nx.generate_adjlist(G):
              file.write(line + '\n')
      )" ) + "\"";

  const std::string command = emp::format_string(
    command_template, n, radius, dim, tmpfile.c_str()
  );

  uitsl::err_verify( std::system( command.c_str() ) );

  return netuit::make_adjacency_file_topology( tmpfile );

}

template<typename Radius=std::deci, size_t Dim=2>
struct SoftRandomGeometricTopologyFactory {

  Topology operator()(const size_t cardinality) const {

    return make_soft_random_geometric_topology(
      cardinality,
      static_cast<double>( Radius::num ) / Radius::den,
      Dim
    );

  }

  netuit::Topology operator()(const emp::vector<size_t>& cardinality) const {
    emp_assert(cardinality.size() == 1);
    return make_soft_random_geometric_topology(
      cardinality.front(),
      static_cast<double>( Radius::num ) / Radius::den,
      Dim
    );
  }

  static std::string GetName() { return "Soft Random Geometric Topology"; }

  static std::string GetSlug() { return "soft_random_geometric"; }

};

} // namespace netuit

#endif // #ifndef NETUIT_ARRANGE_SOFTRANDOMGEOMETRICTOPOLOGYFACTORY_HPP_INCLUDE
