#pragma once
#ifndef NETUIT_ASSIGN_ASSIGNPERFECTHYPERCUBE_HPP_INCLUDE
#define NETUIT_ASSIGN_ASSIGNPERFECTHYPERCUBE_HPP_INCLUDE

#include <functional>
#include <numeric>
#include <stddef.h>

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/Empirical/include/emp/math/math.hpp"

#include "../../uitsl/math/is_perfect_hypercube.hpp"
#include "../../uitsl/math/mapping_utils.hpp"

namespace netuit {

template<typename RETURN_TYPE>
struct AssignPerfectHypercube {

  uitsl::Dims item_dims;
  uitsl::Dims partition_dims;

  AssignPerfectHypercube(
    const size_t n_dims, const size_t n_items, const size_t n_partitions
  ) : item_dims( n_dims, std::pow( n_items, 1.0/n_dims ) )
  , partition_dims(n_dims, std::pow( n_partitions, 1.0/n_dims ) ) {

    emp_assert( uitsl::is_perfect_hypercube( n_items, n_dims) );
    emp_assert( uitsl::is_perfect_hypercube( n_partitions, n_dims) );
    emp_assert( std::accumulate(
      std::begin(item_dims), std::end(item_dims), 1ul, std::multiplies<size_t>{}
    ) == n_items );
    emp_assert( std::accumulate(
      std::begin(partition_dims), std::end(partition_dims),
      1ul, std::multiplies<size_t>{}
    ) == n_partitions );
  }

  RETURN_TYPE operator()(const size_t& node_id) {

    // get item coordinates
    auto coordinates = uitsl::linear_decode( node_id, item_dims );

    emp_assert( item_dims.front() % partition_dims.front() == 0 );

    // transform into partition coordinates
    for (auto& coord : coordinates) {
      coord /= ( item_dims.front() / partition_dims.front() );
    }
    // get partition id
    const auto res = uitsl::linear_encode( coordinates, partition_dims );

    emp_assert( res < std::accumulate(
      std::begin(partition_dims), std::end(partition_dims),
      1ul, std::multiplies<size_t>{}
    ), node_id, res, coordinates, uitsl::linear_decode( node_id, item_dims ), item_dims, partition_dims );

    return res;
  }

};

} // namespace netuit

#endif // #ifndef NETUIT_ASSIGN_ASSIGNPERFECTHYPERCUBE_HPP_INCLUDE
