#pragma once
#ifndef DISH2_INTROSPECTION_COUNT_UNIQUE_STINT_ROOT_IDS_HPP_INCLUDE
#define DISH2_INTROSPECTION_COUNT_UNIQUE_STINT_ROOT_IDS_HPP_INCLUDE

#include <set>

#include "../world/ThreadWorld.hpp"

#include "get_unique_stint_root_ids.hpp"

namespace dish2 {

template< typename Spec >
size_t count_unique_stint_root_ids( const dish2::ThreadWorld<Spec>& world ) {

  return dish2::get_unique_stint_root_ids<Spec>( world ).size();

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_COUNT_UNIQUE_STINT_ROOT_IDS_HPP_INCLUDE
