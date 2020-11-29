#pragma once
#ifndef DISH2_DEBUG_LOGENTRY_HPP_INCLUDE
#define DISH2_DEBUG_LOGENTRY_HPP_INCLUDE

#include <string>

#include "LogEntryType.hpp"

namespace dish2 {

struct LogEntry {

  dish2::LogEntryType type;
  std::string name;
  std::string detail;

};

} // namespace dish2

#endif // #ifndef DISH2_DEBUG_LOGENTRY_HPP_INCLUDE
