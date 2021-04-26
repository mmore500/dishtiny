#pragma once
#ifndef UITSL_ALGORITHM_TRANSFORM_IF_HPP_INCLUDE
#define UITSL_ALGORITHM_TRANSFORM_IF_HPP_INCLUDE

namespace uitsl {

// adapted from https://arne-mertz.de/2019/05/algorithms-and-the-kiss-principle/
template <typename InIter, typename OutIter, typename UnaryOp, typename Pred>
OutIter transform_if(
  InIter first, InIter last, OutIter result, UnaryOp unaryOp, Pred pred
) {

  for(; first != last; ++first) if(pred(*first)) {
    *result = unaryOp(*first);
    ++result;
  }

  return result;

}

} // namespace uitsl

#endif // #ifndef UITSL_ALGORITHM_TRANSFORM_IF_HPP_INCLUDE
