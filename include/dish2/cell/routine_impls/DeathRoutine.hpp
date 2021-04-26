#pragma once
#ifndef DISH2_CELL_ROUTINE_IMPLS_DEATHROUTINE_HPP_INCLUDE
#define DISH2_CELL_ROUTINE_IMPLS_DEATHROUTINE_HPP_INCLUDE

#include <algorithm>

#include "../../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"

#include "../../debug/LogScope.hpp"
#include "../../enum/CauseOfDeath.hpp"

#include "../cardinal_iterators/EpochWrapper.hpp"
#include "../cardinal_iterators/IsAliveWrapper.hpp"
#include "../cardinal_iterators/MostRecentCauseOfDeathWrapper.hpp"


namespace dish2 {

template <class Spec>
void Cell<Spec>::DeathRoutine(
  const dish2::CauseOfDeath cause_of_death
) {

  const dish2::LogScope guard{ "death routine", "TODO", 3 };

  running_logs.Record( dish2::DeathEvent<Spec>{
    cause_of_death, GetPeripherality()
  } );

  // check is alive consistency
  emp_assert(( std::set< typename dish2::IsAliveWrapper<Spec>::value_type >(
    begin<dish2::IsAliveWrapper<Spec>>(),
    end<dish2::IsAliveWrapper<Spec>>()
  ).size() == 1 ));

  // check epoch consistency
  emp_assert(( std::set< typename dish2::EpochWrapper<Spec>::value_type >(
    begin<dish2::EpochWrapper<Spec>>(),
    end<dish2::EpochWrapper<Spec>>()
  ).size() == 1 ));

  HeirPayoutRoutine();

  const auto epoch = *begin<dish2::EpochWrapper<Spec>>();

  Clear();

  std::fill(
    begin<dish2::EpochWrapper<Spec>>(),
    end<dish2::EpochWrapper<Spec>>(),
    epoch
  );

  emp_assert( std::none_of(
    begin<dish2::IsAliveWrapper<Spec>>(),
    end<dish2::IsAliveWrapper<Spec>>(),
    std::identity
  ) );

  // check is alive consistency
  emp_assert(( std::set< typename dish2::IsAliveWrapper<Spec>::value_type >(
    begin<dish2::IsAliveWrapper<Spec>>(),
    end<dish2::IsAliveWrapper<Spec>>()
  ).size() == 1 ));

  // check epoch consistency
  emp_assert(( std::set< typename dish2::EpochWrapper<Spec>::value_type >(
    begin<dish2::EpochWrapper<Spec>>(),
    end<dish2::EpochWrapper<Spec>>()
  ).size() == 1 ));

  std::fill(
    begin<dish2::MostRecentCauseOfDeathWrapper<Spec>>(),
    end<dish2::MostRecentCauseOfDeathWrapper<Spec>>(),
    static_cast<char>( cause_of_death )
  );

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_ROUTINE_IMPLS_DEATHROUTINE_HPP_INCLUDE
