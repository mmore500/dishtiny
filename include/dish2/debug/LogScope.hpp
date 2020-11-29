#pragma once
#ifndef DISH2_DEBUG_LOGSCOPE_HPP_INCLUDE
#define DISH2_DEBUG_LOGSCOPE_HPP_INCLUDE

#include <string>

#include "enable.hpp"
#include "entry_types.hpp"
#include "log_event.hpp"
#include "log_stack.hpp"
#include "log_stack_dispatcher.hpp"

namespace dish2 {

struct LogScope {
#ifdef DISH2_LOG_ENABLE

  std::string name;
  std::string description;

  LogScope(
    const std::string& name_,
    const std::string& description_=""
  ) : name( name_ )
  , description( description_ ) {

    dish2::log_event({
      dish2::scope_begin,
      name,
      description
    });

    dish2::log_stack.emplace_back(
      name,
      description
    );

    for (const auto& fun : log_stack_dispatcher ) fun( dish2::log_stack );

  }

  ~LogScope() {

    dish2::log_stack.pop_back();

    dish2::log_event({
      dish2::scope_end,
      name,
      description
    });

    for (const auto& fun : log_stack_dispatcher ) fun( dish2::log_stack );

  }

#endif // #ifdef DISH2_LOG_ENABLE
};

} // namespace dish2

#endif // #ifndef DISH2_DEBUG_LOGSCOPE_HPP_INCLUDE
