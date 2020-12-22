#pragma once
#ifndef DISH2_ALGORITHM_TEST_INSTRUCTION_FOR_PHENOTYPIC_NEUTRALITY_HPP_INCLUDE
#define DISH2_ALGORITHM_TEST_INSTRUCTION_FOR_PHENOTYPIC_NEUTRALITY_HPP_INCLUDE

#include "../cell/Cell.hpp"
#include "../debug/entry_types.hpp"
#include "../debug/log_event.hpp"
#include "../debug/LogScope.hpp"
#include "../genome/Genome.hpp"
#include "../world/ThreadWorld.hpp"

#include "detect_phenotypic_divergence.hpp"

namespace dish2 {

template< typename Spec >
bool test_instruction_for_phenotypic_neutrality(
  const dish2::Genome<Spec>& genome, const size_t instruction_idx
) {

  const dish2::LogScope guard( "evaluating instruction ", instruction_idx );

  auto nopout = genome;
  nopout.program[ instruction_idx ].NopOut();

  return dish2::detect_phenotypic_divergence<Spec>( genome, nopout );

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_TEST_INSTRUCTION_FOR_PHENOTYPIC_NEUTRALITY_HPP_INCLUDE
