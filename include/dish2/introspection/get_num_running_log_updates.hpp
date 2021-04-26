#pragma once
#ifndef DISH2_INTROSPECTION_GET_NUM_RUNNING_LOG_UPDATES_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_NUM_RUNNING_LOG_UPDATES_HPP_INCLUDE

#include <algorithm>

#include "../config/running_log_duration_in_updates.hpp"

namespace dish2 {

template< typename Spec >
size_t get_num_running_log_updates( const dish2::ThreadWorld<Spec>& world ) {

  return std::min(
    dish2::running_log_duration_in_updates(), world.GetUpdate()
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_NUM_RUNNING_LOG_UPDATES_HPP_INCLUDE
