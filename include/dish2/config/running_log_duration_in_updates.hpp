#pragma once
#ifndef DISH2_CONFIG_RUNNING_LOG_DURATION_IN_UPDATES_HPP_INCLUDE
#define DISH2_CONFIG_RUNNING_LOG_DURATION_IN_UPDATES_HPP_INCLUDE

#include "cfg.hpp"

namespace dish2 {

size_t running_log_duration_in_updates() {
  return cfg.RUNNING_LOG_DURATION() * cfg.RUNNING_LOG_PURGE_SERVICE_FREQUENCY();
}

} // namespace dish2

#endif // #ifndef DISH2_CONFIG_RUNNING_LOG_DURATION_IN_UPDATES_HPP_INCLUDE
