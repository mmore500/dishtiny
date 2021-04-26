#pragma once
#ifndef DISH2_DEBUG_LOG_EVENT_HPP_INCLUDE
#define DISH2_DEBUG_LOG_EVENT_HPP_INCLUDE

#include <limits>

#ifdef __EMSCRIPTEN__
#include <emscripten/threading.h>
#endif

#include "enable.hpp"
#include "log_event_dispatcher.hpp"
#include "log_level.hpp"
#include "log_stack.hpp"

namespace dish2 {

void log_event( const dish2::LogEntry& entry ) {
#ifdef DISH2_LOG_ENABLE

  if ( dish2::log_level >= entry.type.verbosity ) {
    for (const auto& fun : log_event_dispatcher ) {
      fun( entry, dish2::log_stack.size() );
    }
  }

  // if log level is 10, wait until signalled to continue
  if ( dish2::log_level == 10 ) {

    #ifdef __EMSCRIPTEN__
    // adapted from https://github.com/emscripten-core/emscripten/blob/2bca083cbbd5a4133db61fbd74d04f7feecfa907/tests/pthread/test_futex_wake_all.cpp
    while ( emscripten_atomic_load_u32(&dish2::log_futex_val) == 0 ) {
      // If the given memory address contains value val, puts the calling
      // thread to sleep waiting for that address to be notified.
      emscripten_futex_wait(
        &dish2::log_futex_val, 0, std::numeric_limits<double>::infinity()
      );
    }

    if (
      emscripten_atomic_load_u32(&dish2::log_futex_val)
      == dish2::log_futex_release
    ) {
      dish2::log_level = -2;
      emscripten_atomic_store_u32( &dish2::log_futex_val, 1 );
    }

    emscripten_atomic_sub_u32( &dish2::log_futex_val, 1 );

    #else

    emp_assert(false, "native log wait unimplemented!");

    #endif // #ifdef __EMSCRIPTEN__
  }

#endif // #ifdef DISH2_LOG_ENABLE
}

} // namespace dish2

#endif // #ifndef DISH2_DEBUG_LOG_EVENT_HPP_INCLUDE
