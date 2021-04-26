#pragma once
#ifndef DISH2_INTROSPECTION_GET_NUM_RUNNING_LOG_LIVE_CARDINAL_UPDATES_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_NUM_RUNNING_LOG_LIVE_CARDINAL_UPDATES_HPP_INCLUDE

#include "../config/running_log_duration_in_updates.hpp"

#include "count_live_cardinals.hpp"
#include "get_num_running_log_updates.hpp"

namespace dish2 {

template< typename Spec >
size_t get_num_running_log_live_cardinal_updates(
  const dish2::ThreadWorld<Spec>& world
) {

  const size_t num_log_updates = dish2::get_num_running_log_updates<Spec>( world );
  const size_t num_live_cardinal_updates
    = num_log_updates * dish2::count_live_cardinals<Spec>( world );
  return num_live_cardinal_updates;

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_NUM_RUNNING_LOG_LIVE_CARDINAL_UPDATES_HPP_INCLUDE
