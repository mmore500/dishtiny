#pragma once
#ifndef DISH2_DEBUG_LOG_STACK_DISPATCHER_HPP_INCLUDE
#define DISH2_DEBUG_LOG_STACK_DISPATCHER_HPP_INCLUDE

#include <functional>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "log_stack.hpp"

namespace dish2 {

thread_local emp::vector<
  std::function< void( const log_stack_t& log_stack ) >
> log_stack_dispatcher{};

} // namespace dish2

#endif // #ifndef DISH2_DEBUG_LOG_STACK_DISPATCHER_HPP_INCLUDE
