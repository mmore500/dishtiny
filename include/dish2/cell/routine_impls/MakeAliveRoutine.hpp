#pragma once
#ifndef DISH2_CELL_ROUTINE_IMPLS_MAKEALIVEROUTINE_HPP_INCLUDE
#define DISH2_CELL_ROUTINE_IMPLS_MAKEALIVEROUTINE_HPP_INCLUDE

#include <algorithm>
#include <set>

#include "../cardinal_iterators/CpuWrapper.hpp"
#include "../cardinal_iterators/EpochWrapper.hpp"
#include "../cardinal_iterators/IsAliveWrapper.hpp"
#include "../cardinal_iterators/KinGroupIDViewWrapper.hpp"

namespace dish2 {

template <class Spec>
void Cell<Spec>::MakeAliveRoutine() {

  const size_t epoch = *begin<dish2::EpochWrapper<Spec>>();

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

  // check kin group ID consistency
  emp_assert((
    std::set< typename dish2::KinGroupIDViewWrapper<Spec>::value_type >(
      begin<dish2::KinGroupIDViewWrapper<Spec>>(),
      end<dish2::KinGroupIDViewWrapper<Spec>>()
    ).size() == 1
  ));

  std::for_each(
    begin<dish2::KinGroupIDViewWrapper<Spec>>(),
    end<dish2::KinGroupIDViewWrapper<Spec>>(),
    [this](auto& kgiv){ std::copy(
      std::begin( genome->kin_group_id.data ),
      std::end( genome->kin_group_id.data ),
      std::begin( kgiv )
    ); }
  );

  // check kin group ID consistency
  emp_assert((
    std::set< typename dish2::KinGroupIDViewWrapper<Spec>::value_type >(
      begin<dish2::KinGroupIDViewWrapper<Spec>>(),
      end<dish2::KinGroupIDViewWrapper<Spec>>()
    ).size() == 1
  ));

  // set up kin group ages
  std::for_each(
    begin<dish2::KinGroupAgeWrapper<Spec>>(),
    end<dish2::KinGroupAgeWrapper<Spec>>(),
    [this, epoch]( auto& kin_group_age ){
      for( size_t lev{}; lev < kin_group_age.GetSize(); ++lev ) {
        kin_group_age.Get( lev ) = epoch - std::min(
          genome->kin_group_epoch_stamps.GetBuffer()[ lev ],
          epoch
        );
      }
    }
  );


  // load program onto all CPUs
  std::for_each(
    begin<dish2::CpuWrapper<Spec>>(),
    end<dish2::CpuWrapper<Spec>>(),
    [this](auto& cpu){ cpu.InitializeAnchors( genome->program ); }
  );


}

} // namespace dish2

#endif // #ifndef DISH2_CELL_ROUTINE_IMPLS_MAKEALIVEROUTINE_HPP_INCLUDE
