#pragma once
#ifndef NETUIT_ARRANGE_TOROIDALTOPOLOGYFACTORY_HPP_INCLUDE
#define NETUIT_ARRANGE_TOROIDALTOPOLOGYFACTORY_HPP_INCLUDE

#include <algorithm>
#include <cmath>
#include <list>
#include <numeric>
#include <set>
#include <tuple>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/datastructs/tuple_utils.hpp"

#include "../../uitsl/math/is_perfect_hypercube.hpp"
#include "../../uitsl/math/mapping_utils.hpp"
#include "../../uitsl/math/math_utils.hpp"
#include "../../uitsl/utility/UIDMap.hpp"

#include "../topology/TopoEdge.hpp"
#include "../topology/Topology.hpp"
#include "../topology/TopoNode.hpp"

#include "ToroidalGridTopologyFactory.hpp"

namespace netuit {

inline netuit::Topology make_toroidal_topology(
  const uitsl::Dims& dim_cardinality
) {

  // special case: if two-dimensional square, custom construct so it's easy
  // to visualize
  if ( dim_cardinality.size() == 2 && std::set<size_t>(
    std::begin( dim_cardinality ), std::end( dim_cardinality )
  ).size() == 1 ) return netuit::make_toroidal_grid_topology( dim_cardinality );

  /*
  * goal
  * make toroidal topology (such that opposite edges are the same edge)
  */
  const size_t cardinality = std::accumulate(
    dim_cardinality.begin(),
    dim_cardinality.end(),
    1ul,
    std::multiplies<size_t>()
  );

  emp::vector<netuit::TopoNode> nodes(cardinality);
  uitsl::UIDMap<size_t> node_edge_map;

  auto get_neighbor = [&dim_cardinality](uitsl::Point p, const size_t dim, const int n) -> uitsl::Point {
    p[dim] = uitsl::circular_index(p[dim], dim_cardinality[dim], n);
    return p;
  };

  // returns a vector containing all neighbors
  auto get_neighbors = [&get_neighbor](const uitsl::Point& p) {
    emp::vector<uitsl::Point> neighbors;

    for (size_t i{}; i < p.size(); ++i) {
      neighbors.push_back(get_neighbor(p, i, +1));
    }

    for (size_t i{}; i < p.size(); ++i) {
      // go through dimensions in reverse order
      const size_t j = p.size() - 1 - i;
      neighbors.push_back(get_neighbor(p, j, -1));
    }

    return neighbors;
  };

  for (size_t neigh{}; neigh < dim_cardinality.size() * 2; ++neigh) {

    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
      const size_t my_idx = std::distance(nodes.begin(), it);
      const auto my_point = uitsl::linear_decode(my_idx, dim_cardinality);

      const auto neighbor_point = get_neighbors(my_point)[neigh];
      const size_t neighbor_idx = uitsl::linear_encode(
        neighbor_point, dim_cardinality
      );

      it->AddOutput(node_edge_map[{true, my_idx, neighbor_idx}]);

    }

  }

  for (size_t neigh{}; neigh < dim_cardinality.size() * 2; ++neigh) {

    for (auto it = nodes.rbegin(); it != nodes.rend(); ++it) {
      const size_t my_idx
        = nodes.size() - std::distance(nodes.rbegin(), it) - 1;
      const auto my_point = uitsl::linear_decode(my_idx, dim_cardinality);

      const auto neighbor_point = get_neighbors(my_point)[neigh];
      const size_t neighbor_idx = uitsl::linear_encode(
        neighbor_point, dim_cardinality
      );

      it->AddInput(node_edge_map[{false, neighbor_idx, my_idx}]);

    }

  }

  return nodes;
}

struct ToroidalTopologyFactory {

  netuit::Topology operator()(const size_t cardinality) const {
    return make_toroidal_topology({cardinality});
  }

  netuit::Topology operator()(const uitsl::Dims& dim_cardinality) const {
    return make_toroidal_topology(dim_cardinality);
  }

  static std::string GetName() { return "Toroidal Topology"; }

  static std::string GetSlug() { return "toroidal"; }

};

} // namespace netuit

#endif // #ifndef NETUIT_ARRANGE_TOROIDALTOPOLOGYFACTORY_HPP_INCLUDE
