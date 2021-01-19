#pragma once
#ifndef DISH2_ALGORITHM_TEST_INSTRUCTIONS_FOR_PHENOTYPIC_NEUTRALITY_HPP_INCLUDE
#define DISH2_ALGORITHM_TEST_INSTRUCTIONS_FOR_PHENOTYPIC_NEUTRALITY_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../cell/Cell.hpp"
#include "../debug/entry_types.hpp"
#include "../debug/log_event.hpp"
#include "../debug/LogScope.hpp"
#include "../genome/Genome.hpp"
#include "../world/ThreadWorld.hpp"

#include "detect_phenotypic_divergence.hpp"

namespace dish2 {

template< typename Spec >
bool test_instructions_for_phenotypic_neutrality(
  const dish2::Genome<Spec>& genome,
  const size_t inst_idx,
  const size_t nop_length=1
) {

  const dish2::LogScope guard( emp::to_string("evaluating inst ", inst_idx) );

  auto nopout = genome;
  for (
    size_t idx = inst_idx;
    idx < std::min(inst_idx + nop_length, nopout.program.size());
    ++idx
  ) nopout.program[ inst_idx ].NopOut();

  return dish2::detect_phenotypic_divergence<Spec>( genome, nopout ) == false;

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_TEST_INSTRUCTIONS_FOR_PHENOTYPIC_NEUTRALITY_HPP_INCLUDE
