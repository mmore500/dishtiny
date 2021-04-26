#pragma once
#ifndef DISH2_UTILITY_MAKE_HISTOGRAM_HPP_INCLUDE
#define DISH2_UTILITY_MAKE_HISTOGRAM_HPP_INCLUDE

#include <algorithm>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

namespace dish2 {

template< typename Iterator, typename UnaryOp>
auto make_histogram(
  const Iterator begin, const Iterator end, UnaryOp transform=std::identity
) {

  using T = typename Iterator::value_type;
  using K = typename std::result_of<UnaryOp(T)>::type;

  std::unordered_map< K, size_t > res;

  std::for_each( begin, end, [&]( const T& val ){
    ++res[ transform( val ) ];
  } );

  return res;

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_MAKE_HISTOGRAM_HPP_INCLUDE
