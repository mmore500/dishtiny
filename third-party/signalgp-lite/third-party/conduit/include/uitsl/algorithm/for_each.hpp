#pragma once
#ifndef UITSL_ALGORITHM_FOR_EACH_HPP_INCLUDE
#define UITSL_ALGORITHM_FOR_EACH_HPP_INCLUDE

#include <algorithm>

namespace uitsl {

template<class InputIt1, class InputIt2, class BinaryOperation>
void for_each(
  InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryOperation binary_op
) {
  while (first1 != last1) binary_op(*first1++, *first2++);
}

template<class InputIt1, class InputIt2, class InputIt3, class BinaryOperation>
void for_each(
  InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt3 first3,
  BinaryOperation trinary_op
) {
  while (first1 != last1) trinary_op(*first1++, *first2++, *first3++);
}

template<
  class InputIt1, class InputIt2, class InputIt3, class InputIt4,
  class BinaryOperation
>
void for_each(
  InputIt1 first1, InputIt1 last1,
  InputIt2 first2, InputIt3 first3, InputIt4 first4,
  BinaryOperation quaternary_op
) {
  while (first1 != last1) quaternary_op(
    *first1++, *first2++, *first3++, *first4++
  );
}

} // namespace uitsl

#endif // #ifndef UITSL_ALGORITHM_FOR_EACH_HPP_INCLUDE
