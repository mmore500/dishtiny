#pragma once
#ifndef DISH2_ALGORITHM_NOP_OUT_PHENOTYPICALLY_NEUTRAL_INSTRUCTIONS_HPP_INCLUDE
#define DISH2_ALGORITHM_NOP_OUT_PHENOTYPICALLY_NEUTRAL_INSTRUCTIONS_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/morph/nop_out_instructions.hpp"

#include "../debug/LogScope.hpp"
#include "../genome/Genome.hpp"
#include "../world/ThreadWorld.hpp"

#include "test_instructions_for_phenotypic_neutrality.hpp"

namespace dish2 {

template< typename Spec >
dish2::Genome<Spec> nop_out_phenotypically_neutral_instructions(
  dish2::Genome<Spec> genome, const size_t nop_length=1
) {

  const dish2::LogScope guard{ "evaluating instruction-by-instruction" };
  std::cout
    << "evaluating " << genome.program.size() << " instructions "
    << nop_length << " at a time" << std::endl;

  using sgpl_spec_t = typename Spec::sgpl_spec_t;

  emp::vector< char > should_nop( genome.program.size() );

  #pragma omp parallel for
  for (size_t idx = 0; idx < genome.program.size(); idx += nop_length) {

    #ifdef __EMSCRIPTEN__
    const dish2::LogScope guard3{
      emp::to_string("evaluating instruction ", idx)
    };
    #endif // #ifdef __EMSCRIPTEN__

    const bool res = dish2::test_instructions_for_phenotypic_neutrality< Spec >(
      genome, idx, nop_length
    );

    for (size_t i{}; i < nop_length; ++i) if ( idx + i < should_nop.size() ) {
      should_nop[idx + i] = res;
      std::cout << ( res ? "x" : "o" ) << std::flush;
    }

  }

  std::cout << " done" << std::endl;

  genome.program
    = sgpl::nop_out_instructions< sgpl_spec_t >( genome.program, should_nop );

  return genome;

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_NOP_OUT_PHENOTYPICALLY_NEUTRAL_INSTRUCTIONS_HPP_INCLUDE
