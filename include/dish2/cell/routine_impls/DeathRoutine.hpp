#pragma once
#ifndef DISH2_CELL_ROUTINE_IMPLS_DEATHROUTINE_HPP_INCLUDE
#define DISH2_CELL_ROUTINE_IMPLS_DEATHROUTINE_HPP_INCLUDE

#include <algorithm>

#include "../cardinal_iterators/IsAliveWrapper.hpp"

namespace dish2 {

template <class Spec>
void Cell<Spec>::DeathRoutine() {

  HeirPayoutRoutine();

  Clear();

  std::fill(
    begin<dish2::IsAliveWrapper<Spec>>(),
    end<dish2::IsAliveWrapper<Spec>>(),
    false
  );

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_ROUTINE_IMPLS_DEATHROUTINE_HPP_INCLUDE
