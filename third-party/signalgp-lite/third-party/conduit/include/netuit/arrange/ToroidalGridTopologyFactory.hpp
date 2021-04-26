#pragma once
#ifndef NETUIT_ARRANGE_TOROIDALGRIDTOPOLOGYFACTORY_HPP_INCLUDE
#define NETUIT_ARRANGE_TOROIDALGRIDTOPOLOGYFACTORY_HPP_INCLUDE

#include <algorithm>
#include <cmath>
#include <list>
#include <numeric>
#include <set>
#include <tuple>

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/datastructs/tuple_utils.hpp"

#include "../../uitsl/math/is_perfect_hypercube.hpp"
#include "../../uitsl/math/mapping_utils.hpp"
#include "../../uitsl/math/math_utils.hpp"
#include "../../uitsl/utility/UIDMap.hpp"

#include "../topology/TopoEdge.hpp"
#include "../topology/Topology.hpp"
#include "../topology/TopoNode.hpp"

namespace netuit {


/// Special case for well-behaved two-dimensional square grid
/// that's easy to visualize.
inline netuit::Topology make_toroidal_grid_topology(
  const uitsl::Dims& dim_cardinality
) {

  emp_assert( dim_cardinality.size() == 2 ); // two-dimensional
  emp_assert( std::set<size_t>(
    std::begin( dim_cardinality ), std::end( dim_cardinality )
  ).size() == 1 ); // square

  const size_t dimension = dim_cardinality.front();

  const size_t cardinality = std::accumulate(
    dim_cardinality.begin(),
    dim_cardinality.end(),
    1ul,
    std::multiplies<size_t>{}
  );

  emp::vector<netuit::TopoNode> nodes(cardinality);
  uitsl::UIDMap<size_t> node_edge_map;

  auto get_north_output_id = [cardinality](const size_t idx){
    return cardinality * 0 + idx;
  };
  auto get_west_output_id = [cardinality](const size_t idx){
    return cardinality * 1 + idx;
  };
  auto get_east_output_id = [cardinality](const size_t idx){
    return cardinality * 2 + idx;
  };
  auto get_south_output_id = [cardinality](const size_t idx){
    return cardinality * 3 + idx;
  };

  auto get_north_input_id = [&](const size_t idx){
    // northern neighbor's southern input edge id
    return get_south_output_id(
      (idx + cardinality - dimension) % cardinality
    );
  };
  auto get_south_input_id = [&](const size_t idx){
    // southern neighbor's northern input edge id
    return get_north_output_id(
      (idx + dimension) % cardinality
    );
  };
  auto get_east_input_id = [&](const size_t idx){
    const size_t first_idx_in_row = (idx / dimension) * dimension;
    const size_t idx_in_row = idx % dimension;

    // eastern neighbor's western input id
    return get_west_output_id(
      first_idx_in_row + (idx_in_row + 1) % dimension
    );
  };
  auto get_west_input_id = [&](const size_t idx){
    const size_t first_idx_in_row = (idx / dimension) * dimension;
    const size_t idx_in_row = idx % dimension;

    // western neighbor's eastern input id
    return get_east_output_id(
      first_idx_in_row + (idx_in_row + dimension - 1) % dimension
    );
  };

  for (auto it = nodes.begin(); it != nodes.end(); ++it) {
    const size_t idx = std::distance(nodes.begin(), it);
    it->AddInput( get_north_input_id( idx ) );
    it->AddInput( get_west_input_id( idx ) );
    it->AddInput( get_east_input_id( idx ) );
    it->AddInput( get_south_input_id( idx ) );

    it->AddOutput( get_north_output_id( idx ) );
    it->AddOutput( get_west_output_id( idx ) );
    it->AddOutput( get_east_output_id( idx ) );
    it->AddOutput( get_south_output_id( idx ) );
  }

  return nodes;

}

struct ToroidalGridTopologyFactory {

  netuit::Topology operator()(const size_t cardinality) const {
    emp_assert( uitsl::is_perfect_hypercube( cardinality, 2 ) );
    const size_t dimension = static_cast<size_t>( std::sqrt(cardinality) );
    return make_toroidal_grid_topology( { dimension, dimension } );
  }

  netuit::Topology operator()(const uitsl::Dims& dim_cardinality) const {
    return make_toroidal_grid_topology(dim_cardinality);
  }

  static std::string GetName() { return "Toroidal Grid Topology"; }

  static std::string GetSlug() { return "toroidal_grid"; }

};

} // namespace netuit

#endif // #ifndef NETUIT_ARRANGE_TOROIDALGRIDTOPOLOGYFACTORY_HPP_INCLUDE
