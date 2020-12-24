#pragma once
#ifndef DISH2_ALGORITHM_NOP_OUT_PHENOTYPICALLY_NEUTRAL_MODULES_HPP_INCLUDE
#define DISH2_ALGORITHM_NOP_OUT_PHENOTYPICALLY_NEUTRAL_MODULES_HPP_INCLUDE

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/morph/nop_out_modules.hpp"

#include "../debug/entry_types.hpp"
#include "../debug/log_event.hpp"
#include "../debug/LogScope.hpp"
#include "../genome/Genome.hpp"

#include "test_module_for_phenotypic_neutrality.hpp"

namespace dish2 {

template< typename Spec >
dish2::Genome<Spec> nop_out_phenotypically_neutral_modules(
  dish2::Genome<Spec> genome
) {

  using sgpl_spec_t = typename Spec::sgpl_spec_t;
  const size_t num_modules = sgpl::count_modules<sgpl_spec_t>(
    genome.program
  );

  const dish2::LogScope guard( "evaluating module-by-module" );
  std::cout << "evaluating " << num_modules << " modules" << std::endl;

  emp::vector< char > should_nop( num_modules );

  #pragma omp parallel for
  for (size_t module_idx = 0; module_idx < num_modules; ++module_idx) {

    #ifdef __EMSCRIPTEN__
    const dish2::LogScope guard(
      emp::to_string("evaluating module ", module_idx)
    );
    #endif // #ifdef __EMSCRITPEN__

    should_nop[module_idx] = dish2::test_module_for_phenotypic_neutrality<Spec>(
      genome, module_idx
    );

    std::cout << ( should_nop[module_idx] ? "x" : "o" ) << std::flush;

  }

  std::cout << " done" << std::endl;

  genome.program
    = sgpl::nop_out_modules< sgpl_spec_t >( genome.program, should_nop );

  return genome;

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_NOP_OUT_PHENOTYPICALLY_NEUTRAL_MODULES_HPP_INCLUDE
