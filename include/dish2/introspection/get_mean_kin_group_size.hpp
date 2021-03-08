#pragma once
#ifndef DISH2_INTROSPECTION_GET_MEAN_KIN_GROUP_SIZE_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_MEAN_KIN_GROUP_SIZE_HPP_INCLUDE

#include <numeric>

#include "get_kin_group_sizes.hpp"

namespace dish2 {

/// @return map of kin group IDs to counts.
template< typename Spec >
double get_mean_kin_group_size(
  const dish2::ThreadWorld<Spec>& world, const size_t lev
) {

  const auto group_counts = dish2::get_kin_group_sizes<Spec>( world, lev );

  return std::accumulate(
    std::begin( group_counts ), std::end( group_counts ),
    double{},
    [](const auto& accumulator, const auto& group_count){
      const auto& [group, count] = group_count;
      return accumulator + count;
    }
  ) / group_counts.size();

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_MEAN_KIN_GROUP_SIZE_HPP_INCLUDE
