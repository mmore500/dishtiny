#pragma once
#ifndef DISH2_UTILITY_MAKE_STRING_KEY_HISTOGRAM_HPP_INCLUDE
#define DISH2_UTILITY_MAKE_STRING_KEY_HISTOGRAM_HPP_INCLUDE

#include <algorithm>
#include <string>
#include <unordered_map>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

namespace dish2 {

template< typename Iterator >
std::unordered_map<std::string, size_t> make_string_key_histogram(
  const Iterator begin, const Iterator end
) {

  using T = typename Iterator::value_type;

  std::unordered_map<std::string, size_t> res;

  std::for_each( begin, end, [&]( const T& val ){
    ++res[ emp::to_string( val ) ];
  } );

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_MAKE_STRING_KEY_HISTOGRAM_HPP_INCLUDE
