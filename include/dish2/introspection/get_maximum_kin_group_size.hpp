#pragma once
#ifndef DISH2_INTROSPECTION_GET_MAXIMUM_KIN_GROUP_SIZE_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_MAXIMUM_KIN_GROUP_SIZE_HPP_INCLUDE

#include <algorithm>
#include <limits>

#include "get_kin_group_sizes.hpp"

namespace dish2 {

/// @return map of kin group IDs to counts.
template< typename Spec >
double get_maximum_kin_group_size(
  const dish2::ThreadWorld<Spec>& world, const size_t lev
) {

  const auto group_counts = dish2::get_kin_group_sizes<Spec>( world, lev );

  if ( group_counts.empty() ) return std::numeric_limits<double>::quiet_NaN();
  else return std::max_element(
    std::begin( group_counts ), std::end( group_counts ),
    []( const auto& left, const auto& right ){
      return left.second < right.second;
    }
  )->second;

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_MAXIMUM_KIN_GROUP_SIZE_HPP_INCLUDE
