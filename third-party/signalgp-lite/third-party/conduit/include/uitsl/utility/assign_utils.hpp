#pragma once
#ifndef UITSL_UTILITY_ASSIGN_UTILS_HPP_INCLUDE
#define UITSL_UTILITY_ASSIGN_UTILS_HPP_INCLUDE

#include <functional>
#include <stddef.h>

namespace uitsl {

template<typename RETURN_TYPE>
struct AssignIntegrated {
  RETURN_TYPE operator()(const size_t & node_id) {
    return 0;
  }
};

template<typename RETURN_TYPE>
struct AssignSegregated {

  RETURN_TYPE operator()(const size_t & node_id) {
    return node_id;
  }

};


template<typename RETURN_TYPE>
struct AssignContiguously {

  const size_t num_groups;
  const size_t num_items;

  AssignContiguously(
    const size_t num_groups_,
    const size_t num_items_
  ) : num_groups(num_groups_)
  , num_items(num_items_)
  { ; }

  RETURN_TYPE operator()(const size_t & node_id) {
    return (node_id * num_groups / num_items) % num_groups;
  }

};

template<typename RETURN_TYPE>
struct AssignRoundRobin {

  const size_t num_groups;
  const size_t chunk;

  AssignRoundRobin(
    const size_t num_groups_,
    const size_t chunk_=1
  ) : num_groups(num_groups_)
  , chunk(chunk_)
  { ; }

  RETURN_TYPE operator()(const size_t & node_id) {
    return node_id / chunk % num_groups;
  }

};

} // namespace uitsl
// TODO assign_randomly
// make a vector of IDs, shuffle it, capture it,
// then return result of indexing into it

// TODO assign_chunkily (rename)
// arrange into n-dimensional volume
// then divvy into n-dimensional subcubes
// (special case of assign_contiguously)

#endif // #ifndef UITSL_UTILITY_ASSIGN_UTILS_HPP_INCLUDE
