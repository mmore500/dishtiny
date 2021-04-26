#pragma once
#ifndef DISH2_ALGORITHM_BATTLESHIP_NOP_OUT_PHENOTYPICALLY_NEUTRAL_INSTRUCTIONS_HPP_INCLUDE
#define DISH2_ALGORITHM_BATTLESHIP_NOP_OUT_PHENOTYPICALLY_NEUTRAL_INSTRUCTIONS_HPP_INCLUDE

#include <algorithm>
#include <iostream>
#include <tuple>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/morph/nop_out_instructions.hpp"

#include "../debug/log_msg.hpp"
#include "../debug/LogScope.hpp"
#include "../debug/log_tee.hpp"
#include "../debug/make_log_entry_boilerplate.hpp"
#include "../genome/Genome.hpp"
#include "../world/ThreadWorld.hpp"

#include "assess_instructions_for_phenotypic_divergence.hpp"

namespace dish2 {

template< typename Spec >
auto battleship_nop_out_phenotypically_neutral_instructions(
  dish2::Genome<Spec> genome, const size_t nop_length=1
) {

  const dish2::LogScope guard{ "evaluating instruction-by-instruction" };
  dish2::log_msg(
    "evaluating ", genome.program.size(), " instructions ",
    nop_length, " at a time"
  );

  using sgpl_spec_t = typename Spec::sgpl_spec_t;

  emp::vector< char > should_nop( genome.program.size() );
  emp::vector< size_t > divergence_updates( genome.program.size() );

  #pragma omp parallel for
  for (size_t idx = 0; idx < genome.program.size(); idx += nop_length) {

    #ifdef __EMSCRIPTEN__
    const dish2::LogScope guard3{
      emp::to_string("evaluating instruction ", idx)
    };
    #endif // #ifdef __EMSCRIPTEN__

    const auto res = dish2::assess_instructions_for_phenotypic_divergence<Spec>(
        genome, idx, nop_length
      );

    dish2::log_tee << dish2::make_log_entry_boilerplate();
    for (size_t i{}; i < nop_length; ++i) if ( idx + i < should_nop.size() ) {
      should_nop[idx + i] = (res == cfg.PHENOTYPIC_DIVERGENCE_N_UPDATES());
      divergence_updates[idx + i] = res;
      if (i == 0) {
        dish2::log_tee << (should_nop[idx + i] ? "x" : "o");
        dish2::log_tee << std::flush;
      }
    }

  }

  dish2::log_tee << " done" << '\n' << '\n';

  genome.program
    = sgpl::nop_out_instructions< sgpl_spec_t >( genome.program, should_nop );

  return std::tuple{genome, divergence_updates};

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_BATTLESHIP_NOP_OUT_PHENOTYPICALLY_NEUTRAL_INSTRUCTIONS_HPP_INCLUDE
