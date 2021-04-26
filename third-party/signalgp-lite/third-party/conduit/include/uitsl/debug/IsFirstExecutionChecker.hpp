#pragma once
#ifndef UITSL_DEBUG_ISFIRSTEXECUTIONCHECKER_HPP_INCLUDE
#define UITSL_DEBUG_ISFIRSTEXECUTIONCHECKER_HPP_INCLUDE

#include <atomic>
#include <stddef.h>

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"

namespace uitsl {

class IsFirstExecutionChecker {

  std::atomic<size_t> flag{false};

public:

  IsFirstExecutionChecker() = default;

  IsFirstExecutionChecker(const uitsl::IsFirstExecutionChecker& other)
  : flag(other.flag.load())
  { ; }

  void Check() { emp_assert(0 == flag++); }

};

} // namespace uitsl

#endif // #ifndef UITSL_DEBUG_ISFIRSTEXECUTIONCHECKER_HPP_INCLUDE
