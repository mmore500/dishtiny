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

  auto prev_genome = *genome;

  DeathRoutine( dish2::CauseOfDeath::fragmentation );

  genome = prev_genome;
  genome->ElapseGeneration( Spec::NLEV, epoch );

  MakeAliveRoutine();

}

} // namespace dish2

#endif // #ifndef DISH2_CELL_ROUTINE_IMPLS_FRAGMENTATIONROUTINE_HPP_INCLUDE
