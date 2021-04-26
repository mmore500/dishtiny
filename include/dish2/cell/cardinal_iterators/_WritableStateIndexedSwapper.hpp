#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS__WRITABLESTATEINDEXEDSWAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS__WRITABLESTATEINDEXEDSWAPPER_HPP_INCLUDE

#include <functional>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/writable_state/WritableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class WritableStateIndexedSwapper {

  std::reference_wrapper<dish2::WritableState<Spec>> data;
  size_t idx;

public:

  WritableStateIndexedSwapper(
    dish2::WritableState<Spec>& data_,
    const size_t idx_
  ) : data( data_ )
  , idx( idx_ )
  {}

  // intentionally not by reference
  void swap( WritableStateIndexedSwapper other ) {
    emp_assert( idx == other.idx );
    data.get().Swap( idx, other.data.get() );
  }

};

// intentionally not by reference
template<typename Spec>
void swap(
  WritableStateIndexedSwapper<Spec> lhs, WritableStateIndexedSwapper<Spec> rhs
) {

  lhs.swap( rhs );

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS__WRITABLESTATEINDEXEDSWAPPER_HPP_INCLUDE
