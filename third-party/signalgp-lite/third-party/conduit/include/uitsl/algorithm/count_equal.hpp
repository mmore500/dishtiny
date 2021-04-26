#pragma once
#ifndef UITSL_ALGORITHM_COUNT_EQUAL_HPP_INCLUDE
#define UITSL_ALGORITHM_COUNT_EQUAL_HPP_INCLUDE

#include <algorithm>

namespace uitsl {

template<class InputIt1, class InputIt2>
size_t count_equal( InputIt1 first1, InputIt1 last1, InputIt2 first2 ) {
  size_t res{};
  while (first1 != last1) res += *first1++ == *first2++;
  return res;
}

} // namespace uitsl

#endif // #ifndef UITSL_ALGORITHM_COUNT_EQUAL_HPP_INCLUDE
