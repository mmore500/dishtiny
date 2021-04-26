#pragma once
#ifndef DISH2_CELL_ROUTINE_IMPLS_FRAGMENTATIONROUTINE_HPP_INCLUDE
#define DISH2_CELL_ROUTINE_IMPLS_FRAGMENTATIONROUTINE_HPP_INCLUDE

#include <algorithm>
#include <set>

#include "../../debug/LogScope.hpp"
#include "../../enum/CauseOfDeath.hpp"

#include "../cardinal_iterators/EpochWrapper.hpp"

#include "DeathRoutine.hpp"
#include "MakeAliveRoutine.hpp"

namespace dish2 {

template <class Spec>
void Cell<Spec>::FragmentationRoutine() {

  const dish2::LogScope guard{ "fragmentation routine", "TODO", 3 };

  const size_t epoch = *begin<dish2::EpochWrapper<Spec>>();

  emp_assert( genome.has_value() );

  const auto prev_genome = genome;

  DeathRoutine( dish2::CauseOfDeath::fragmentation );

  emp_assert( prev_genome.has_value() );

  genome = prev_genome;
  genome->ElapseGeneration( Spec::NLEV, epoch );

  MakeAliveRoutine();

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_ROUTINE_IMPLS_FRAGMENTATIONROUTINE_HPP_INCLUDE
