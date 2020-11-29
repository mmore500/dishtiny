#pragma once
#ifndef DISH2_DEBUG_LOGENTRYTYPE_HPP_INCLUDE
#define DISH2_DEBUG_LOGENTRYTYPE_HPP_INCLUDE

#include <string>

namespace dish2 {

struct LogEntryType {

  int verbosity;
  std::string symbol;
  std::string name;

};

} // namespace dish2

#endif // #ifndef DISH2_DEBUG_LOGENTRYTYPE_HPP_INCLUDE
