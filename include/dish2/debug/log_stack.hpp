#pragma once
#ifndef DISH2_DEBUG_LOG_STACK_HPP_INCLUDE
#define DISH2_DEBUG_LOG_STACK_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "LogStackFrame.hpp"

namespace dish2 {

using log_stack_t = emp::vector<dish2::LogStackFrame>;

#ifdef DISH2_LOG_ENABLE

thread_local log_stack_t log_stack{};

#endif // #ifdef DISH2_LOG_ENABLE

} // namespace dish2

#endif // #ifndef DISH2_DEBUG_LOG_STACK_HPP_INCLUDE
