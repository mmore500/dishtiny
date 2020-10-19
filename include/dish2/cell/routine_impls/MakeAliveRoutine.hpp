#pragma once
#ifndef DISH2_CELL_ROUTINE_IMPLS_MAKEALIVEROUTINE_HPP_INCLUDE
#define DISH2_CELL_ROUTINE_IMPLS_MAKEALIVEROUTINE_HPP_INCLUDE

#include <algorithm>
#include <set>

#include "../cardinal_iterators/CpuWrapper.hpp"
#include "../cardinal_iterators/IsAliveWrapper.hpp"

namespace dish2 {

template <class Spec>
void Cell<Spec>::MakeAliveRoutine() {

  // check is alive consistency
  emp_assert(( std::set< typename dish2::IsAliveWrapper<Spec>::value_type >(
    begin<dish2::IsAliveWrapper<Spec>>(),
    end<dish2::IsAliveWrapper<Spec>>()
  ).size() == 1 ));

  std::fill(
    begin<dish2::IsAliveWrapper<Spec>>(),
    end<dish2::IsAliveWrapper<Spec>>(),
    true
  );

  // check is alive consistency
  emp_assert(( std::set< typename dish2::IsAliveWrapper<Spec>::value_type >(
    begin<dish2::IsAliveWrapper<Spec>>(),
    end<dish2::IsAliveWrapper<Spec>>()
  ).size() == 1 ));

  // load program onto all CPUs
  std::for_each(
    begin<dish2::CpuWrapper<Spec>>(),
    end<dish2::CpuWrapper<Spec>>(),
    [this](auto& cpu){ cpu.InitializeAnchors( genome->program ); }
  );


}

} // namespace dish2

#endif // #ifndef DISH2_CELL_ROUTINE_IMPLS_MAKEALIVEROUTINE_HPP_INCLUDE
