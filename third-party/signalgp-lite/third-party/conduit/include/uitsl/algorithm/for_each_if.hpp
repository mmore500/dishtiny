#pragma once
#ifndef UITSL_ALGORITHM_FOR_EACH_IF_HPP_INCLUDE
#define UITSL_ALGORITHM_FOR_EACH_IF_HPP_INCLUDE

namespace uitsl {

// adapted from https://stackoverflow.com/a/3185232
template<typename Iterator, typename Pred, typename Operation>
void for_each_if(Iterator begin, Iterator end, Pred p, Operation op) {
  for(; begin != end; begin++) if (p(*begin)) op(*begin);
}

} // namespace uitsl

#endif // #ifndef UITSL_ALGORITHM_FOR_EACH_IF_HPP_INCLUDE
