#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS__READABLESTATEINDEXEDSWAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS__READABLESTATEINDEXEDSWAPPER_HPP_INCLUDE

#include <functional>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class ReadableStateIndexedSwapper {

  std::reference_wrapper<dish2::ReadableState<Spec>> data;
  size_t idx;

public:

  ReadableStateIndexedSwapper(
    dish2::ReadableState<Spec>& data_,
    const size_t idx_
  ) : data( data_ )
  , idx( idx_ )
  {}

  // intentionally not by reference
  void swap( ReadableStateIndexedSwapper other ) {
    emp_assert( idx == other.idx );
    data.get().Swap( idx, other.data.get() );
  }

};

// intentionally not by reference
template<typename Spec>
void swap(
  ReadableStateIndexedSwapper<Spec> lhs, ReadableStateIndexedSwapper<Spec> rhs
) {

  lhs.swap( rhs );

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS__READABLESTATEINDEXEDSWAPPER_HPP_INCLUDE
