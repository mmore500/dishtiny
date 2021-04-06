#pragma once
#ifndef DISH2_DEBUG_LOG_MSG_NATIVE_HPP_INCLUDE
#define DISH2_DEBUG_LOG_MSG_NATIVE_HPP_INCLUDE

#include <utility>

#include <unistd.h>

#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "log_tee.hpp"
#include "make_log_entry_boilerplate.hpp"

namespace dish2 {

template< typename... Args >
void log_msg_native( Args&&... args ) {

  dish2::log_tee << dish2::make_log_entry_boilerplate();
  dish2::log_tee << emp::to_string(std::forward<Args>(args)...);
  dish2::log_tee << '\n' << '\n';

}

} // namespace dish2

#endif // #ifndef DISH2_DEBUG_LOG_MSG_NATIVE_HPP_INCLUDE
