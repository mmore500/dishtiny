#pragma once
#ifndef DISH2_CELL_ROUTINE_IMPLS_DEATHROUTINE_HPP_INCLUDE
#define DISH2_CELL_ROUTINE_IMPLS_DEATHROUTINE_HPP_INCLUDE

#include <algorithm>

#include "../cardinal_iterators/IsAliveWrapper.hpp"

namespace dish2 {

template <class Spec>
void Cell<Spec>::DeathRoutine() {

  // check is alive consistency
  emp_assert(( std::set< typename dish2::IsAliveWrapper<Spec>::value_type >(
    begin<dish2::IsAliveWrapper<Spec>>(),
    end<dish2::IsAliveWrapper<Spec>>()
  ).size() == 1 ));

  HeirPayoutRoutine();

  Clear();

  std::fill(
    begin<dish2::IsAliveWrapper<Spec>>(),
    end<dish2::IsAliveWrapper<Spec>>(),
    false
  );

  // check is alive consistency
  emp_assert(( std::set< typename dish2::IsAliveWrapper<Spec>::value_type >(
    begin<dish2::IsAliveWrapper<Spec>>(),
    end<dish2::IsAliveWrapper<Spec>>()
  ).size() == 1 ));

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_ROUTINE_IMPLS_DEATHROUTINE_HPP_INCLUDE
