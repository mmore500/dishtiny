#pragma once
#ifndef DISH2_ALGORITHM_ASSESS_INSTRUCTIONS_FOR_PHENOTYPIC_DIVERGENCE_HPP_INCLUDE
#define DISH2_ALGORITHM_ASSESS_INSTRUCTIONS_FOR_PHENOTYPIC_DIVERGENCE_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/morph/nop_out_instructions.hpp"

#include "../cell/Cell.hpp"
#include "../debug/entry_types.hpp"
#include "../debug/log_event.hpp"
#include "../debug/LogScope.hpp"
#include "../genome/Genome.hpp"
#include "../world/ThreadWorld.hpp"

#include "run_until_phenotypic_divergence.hpp"

namespace dish2 {

template< typename Spec >
size_t assess_instructions_for_phenotypic_divergence(
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
  ) nopout.program[ idx ].NopOut();

  return dish2::run_until_phenotypic_divergence<Spec>( genome, nopout );

}

template< typename Spec >
size_t assess_instructions_for_phenotypic_divergence(
  const dish2::Genome<Spec>& genome,
  const emp::vector<char>& inst_mask,
  const dish2::Genome<Spec>& reference_genome
) {

  using sgpl_spec_t = typename Spec::sgpl_spec_t;
  auto nopout = genome;
  nopout.program = sgpl::nop_out_instructions<sgpl_spec_t>(
    nopout.program, inst_mask
  );

  return dish2::run_until_phenotypic_divergence<Spec>(
    reference_genome, nopout
  );

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_ASSESS_INSTRUCTIONS_FOR_PHENOTYPIC_DIVERGENCE_HPP_INCLUDE
