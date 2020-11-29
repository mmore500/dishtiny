#pragma once
#ifndef DISH2_DEBUG_LOG_EVENT_HPP_INCLUDE
#define DISH2_DEBUG_LOG_EVENT_HPP_INCLUDE

#include "enable.hpp"
#include "log_event_dispatcher.hpp"
#include "log_stack.hpp"

namespace dish2 {

void log_event( const dish2::LogEntry& entry ) {
#ifdef DISH2_LOG_ENABLE

  for (const auto& fun : log_event_dispatcher ) {
    fun( entry, dish2::log_stack.size() );
  }

#endif // #ifdef DISH2_LOG_ENABLE
}

} // namespace dish2

#endif // #ifndef DISH2_DEBUG_LOG_EVENT_HPP_INCLUDE
