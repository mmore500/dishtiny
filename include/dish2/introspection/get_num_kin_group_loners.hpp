#pragma once
#ifndef DISH2_INTROSPECTION_GET_NUM_KIN_GROUP_LONERS_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_NUM_KIN_GROUP_LONERS_HPP_INCLUDE

#include <algorithm>

#include "get_kin_group_sizes.hpp"

namespace dish2 {

/// @return map of kin group IDs to counts.
template< typename Spec >
double get_num_kin_group_loners(
  const dish2::ThreadWorld<Spec>& world, const size_t lev
) {

  const auto group_counts = dish2::get_kin_group_sizes<Spec>( world, lev );

  return std::count_if(
    std::begin( group_counts ), std::end( group_counts ),
    [](const auto& group_count){ return group_count.second == 1; }
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_NUM_KIN_GROUP_LONERS_HPP_INCLUDE
