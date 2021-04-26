#pragma once
#ifndef UITSL_ALGORITHM_TRANSFORM_HPP_INCLUDE
#define UITSL_ALGORITHM_TRANSFORM_HPP_INCLUDE

namespace uitsl {

template<
  class InputIt1, class InputIt2, class InputIt3,
  class OutputIt,
  class TrinaryOperation
>
void transform(
  InputIt1 first1, InputIt1 last1,
  InputIt2 first2, InputIt3 first3,
  OutputIt out_first,
  TrinaryOperation trinary_op
) {
  while (first1 != last1) *out_first++ = trinary_op(
    *first1++, *first2++, *first3++
  );
}

template<
  class InputIt1, class InputIt2, class InputIt3, class InputIt4,
  class OutputIt,
  class QuaternaryOperation
>
void transform(
  InputIt1 first1, InputIt1 last1,
  InputIt2 first2, InputIt3 first3, InputIt4 first4,
  OutputIt out_first,
  QuaternaryOperation quaternary_op
) {
  while (first1 != last1) *out_first++ = quaternary_op(
    *first1++, *first2++, *first3++, *first4++
  );
}

} // namespace uitsl

#endif // #ifndef UITSL_ALGORITHM_TRANSFORM_HPP_INCLUDE
