#pragma once
#ifndef DISH2_DEBUG_LOGSCOPE_HPP_INCLUDE
#define DISH2_DEBUG_LOGSCOPE_HPP_INCLUDE

#include <string>

#include "enable.hpp"
#include "entry_types.hpp"
#include "log_event.hpp"
#include "log_level.hpp"
#include "log_stack_dispatcher.hpp"
#include "log_stack.hpp"

namespace dish2 {

struct LogScope {
#ifdef DISH2_LOG_ENABLE

  std::string name;
  std::string description;
  int verbosity;

  LogScope(
    const std::string& name_,
    const std::string& description_="",
    const int verbosity_=dish2::scope_begin.verbosity
  ) : name( name_ )
  , description( description_ )
  , verbosity( verbosity_ ) {

    auto event = dish2::scope_begin;
    event.verbosity = verbosity;

    dish2::log_event({
      event,
      name,
      description,
    });

    dish2::log_stack.emplace_back(
      name,
      description
    );

    if ( dish2::log_level >= verbosity ) {
      for (const auto& fun : log_stack_dispatcher ) fun( dish2::log_stack );
    }

  }

  ~LogScope() {

    dish2::log_stack.pop_back();

    auto event = dish2::scope_end;
    event.verbosity = verbosity;

    dish2::log_event({
      event,
      name,
      description
    });

    if ( dish2::log_level >= verbosity ) {
      for (const auto& fun : log_stack_dispatcher ) fun( dish2::log_stack );
    }

  }

#else

  template< typename... Args > LogScope(Args&&... args) {}

#endif // #ifdef DISH2_LOG_ENABLE
};

} // namespace dish2

#endif // #ifndef DISH2_DEBUG_LOGSCOPE_HPP_INCLUDE
