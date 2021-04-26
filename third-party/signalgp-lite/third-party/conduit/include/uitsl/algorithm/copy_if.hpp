#pragma once
#ifndef UITSL_ALGORITHM_COPY_IF_HPP_INCLUDE
#define UITSL_ALGORITHM_COPY_IF_HPP_INCLUDE

#include <algorithm>

namespace uitsl {

template<class SourceIt, class InputIt1, class OutputIt, class UnaryPredicate>
OutputIt copy_if(
  SourceIt first, SourceIt last, InputIt1 first1,
  OutputIt d_first, UnaryPredicate pred
) {
  while (first != last) {
    if ( pred(*first1) ) *d_first++ = *first;
    first++;
    first1++;
  }
  return d_first;
}

template<
  class SourceIt, class InputIt1, class InputIt2,
  class OutputIt, class BinaryPredicate
>
OutputIt copy_if(
  SourceIt first, SourceIt last, InputIt1 first1, InputIt2 first2,
  OutputIt d_first, BinaryPredicate pred
) {
  while (first != last) {
    if ( pred(*first1, *first2) ) *d_first++ = *first;
    first++;
    first1++;
    first2++;
  }
  return d_first;
}

template<
  class SourceIt, class InputIt1, class InputIt2, class InputIt3,
  class OutputIt, class TrinaryPredicate
>
OutputIt copy_if(
  SourceIt first, SourceIt last,
  InputIt1 first1, InputIt2 first2, InputIt3 first3,
  OutputIt d_first, TrinaryPredicate pred
) {
  while (first != last) {
    if ( pred(*first1, *first2, *first3) ) *d_first++ = *first;
    first++;
    first1++;
    first2++;
    first3++;
  }
  return d_first;
}

} // namespace uitsl

#endif // #ifndef UITSL_ALGORITHM_COPY_IF_HPP_INCLUDE
