#pragma once
#ifndef UITSL_ALGORITHM_FIND_IF_NTH_HPP_INCLUDE
#define UITSL_ALGORITHM_FIND_IF_NTH_HPP_INCLUDE

#include <algorithm>

namespace uitsl {

// adapted from https://stackoverflow.com/a/17865660
template<typename Iterator, typename Pred, typename Counter>
Iterator find_if_nth( Iterator first, Iterator last, Pred closure, Counter n ) {

  typedef typename std::iterator_traits<Iterator>::reference Tref;

  return std::find_if(
    first,
    last,
    [&](Tref x) { return closure(x) && !(--n); }
  );

}

} // namespace uitsl

#endif // #ifndef UITSL_ALGORITHM_FIND_IF_NTH_HPP_INCLUDE
