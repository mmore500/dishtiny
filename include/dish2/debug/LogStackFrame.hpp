#pragma once
#ifndef DISH2_DEBUG_LOGSTACKFRAME_HPP_INCLUDE
#define DISH2_DEBUG_LOGSTACKFRAME_HPP_INCLUDE

#include <string>

namespace dish2 {

struct LogStackFrame {

  std::string name;
  std::string detail;

  LogStackFrame() = default;

  LogStackFrame( const std::string& name_, const std::string& detail_="" )
  : name( name_ ), detail( detail_ )
  {}

};

} // namespace dish2

#endif // #ifndef DISH2_DEBUG_LOGSTACKFRAME_HPP_INCLUDE
