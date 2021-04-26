#pragma once
#ifndef DISH2_DEBUG_LOGLEVELGUARD_HPP_INCLUDE
#define DISH2_DEBUG_LOGLEVELGUARD_HPP_INCLUDE

#include "log_level.hpp"

namespace dish2 {

struct LogLevelGuard {

  int prev_log_level;

  LogLevelGuard(const int requested_level=9)
  : prev_log_level( std::exchange( dish2::log_level, requested_level ) )
  {}

  ~LogLevelGuard() {
    dish2::log_level = prev_log_level;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_DEBUG_LOGLEVELGUARD_HPP_INCLUDE
