#pragma once
#ifndef DISH2_ALGORITHM_GET_LOWEST_ROOT_HPP_INCLUDE
#define DISH2_ALGORITHM_GET_LOWEST_ROOT_HPP_INCLUDE

#include <algorithm>

namespace dish2 {

template<typename C>
size_t get_lowest_root( const C& population ) {

  const size_t lowest_root = std::min_element(
    std::begin( population ), std::end( population ),
    []( const auto& left_genome, const auto& right_genome ){
      return left_genome.root_id.GetID() < right_genome.root_id.GetID();
    }
  )->root_id.GetID();

  return lowest_root;

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_GET_LOWEST_ROOT_HPP_INCLUDE
