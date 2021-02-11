#pragma once
#ifndef DISH2_INTROSPECTION_GET_MEDIAN_KIN_GROUP_SIZE_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_MEDIAN_KIN_GROUP_SIZE_HPP_INCLUDE

#include <algorithm>
#include <limits>

#include "get_kin_group_sizes.hpp"

namespace dish2 {

/// @return map of kin group IDs to counts.
template< typename Spec >
double get_median_kin_group_size(
  const dish2::ThreadWorld<Spec>& world, const size_t lev
) {

  const auto group_counts = dish2::get_kin_group_sizes<Spec>( world, lev );

  if ( group_counts.empty() ) return std::numeric_limits<double>::quiet_NaN();

  emp::vector< size_t > counts;
  counts.reserve( group_counts.size() );
  std::transform(
    std::begin( group_counts ), std::end( group_counts ),
    std::back_inserter( counts ),
    []( const auto& group_count ){ return group_count.second; }
  );

  const size_t median_index = counts.size() / 2;

  std::nth_element(
    counts.begin(), std::next(counts.begin(), median_index), counts.end()
  );

  return counts[ median_index ];

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_MEDIAN_KIN_GROUP_SIZE_HPP_INCLUDE
