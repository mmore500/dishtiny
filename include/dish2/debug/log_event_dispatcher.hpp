#pragma once
#ifndef DISH2_DEBUG_LOG_EVENT_DISPATCHER_HPP_INCLUDE
#define DISH2_DEBUG_LOG_EVENT_DISPATCHER_HPP_INCLUDE

#include <functional>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "LogEntry.hpp"
#include "LogStackFrame.hpp"

namespace dish2 {

thread_local emp::vector<
  std::function< void( const dish2::LogEntry&, const size_t ) >
> log_event_dispatcher{};

} // namespace dish2

#endif // #ifndef DISH2_DEBUG_LOG_EVENT_DISPATCHER_HPP_INCLUDE
