#pragma once
#ifndef UITSL_UTILITY_STREAMSTRINGIFY_HPP_INCLUDE
#define UITSL_UTILITY_STREAMSTRINGIFY_HPP_INCLUDE

#include <sstream>
#include <string>

namespace uitsl {

template <typename T>
std::string stringstreamify( const T& t ) {
  std::stringstream ss;
  ss << t;
  return ss.str();
}

} // namespace uitsl

#endif // #ifndef UITSL_UTILITY_STREAMSTRINGIFY_HPP_INCLUDE
