#pragma once
#ifndef DISH2_ALGORITHM_BATTLESHIP_NOP_OUT_PHENOTYPICALLY_NEUTRAL_MODULES_HPP_INCLUDE
#define DISH2_ALGORITHM_BATTLESHIP_NOP_OUT_PHENOTYPICALLY_NEUTRAL_MODULES_HPP_INCLUDE

#include <tuple>

#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/morph/nop_out_modules.hpp"

#include "../debug/entry_types.hpp"
#include "../debug/log_event.hpp"
#include "../debug/log_msg.hpp"
#include "../debug/LogScope.hpp"
#include "../debug/log_tee.hpp"
#include "../debug/make_log_entry_boilerplate.hpp"
#include "../genome/Genome.hpp"

#include "assess_module_for_phenotypic_divergence.hpp"

namespace dish2 {

template< typename Spec >
auto battleship_nop_out_phenotypically_neutral_modules(
  dish2::Genome<Spec> genome
) {

  using sgpl_spec_t = typename Spec::sgpl_spec_t;
  const size_t num_modules = sgpl::count_modules<sgpl_spec_t>(
    genome.program
  );

  const dish2::LogScope guard( "evaluating module-by-module" );
  dish2::log_msg("evaluating ", num_modules, " modules");

  emp::vector< char > should_nop( num_modules );
  emp::vector< size_t > divergence_updates( num_modules );

  dish2::log_tee << dish2::make_log_entry_boilerplate();

  #pragma omp parallel for
  for (size_t module_idx = 0; module_idx < num_modules; ++module_idx) {

    #ifdef __EMSCRIPTEN__
    const dish2::LogScope guard(
      emp::to_string("evaluating module ", module_idx)
    );
    #endif // #ifdef __EMSCRITPEN__

    const auto res = dish2::assess_module_for_phenotypic_divergence<Spec>(
      genome, module_idx
    );

    should_nop[module_idx] = ( res == cfg.PHENOTYPIC_DIVERGENCE_N_UPDATES() );
    divergence_updates[module_idx] = res;

    dish2::log_tee << ( should_nop[module_idx] ? "x" : "o" ) << std::flush;

  }

  dish2::log_tee << " done" << '\n' << '\n';

  genome.program
    = sgpl::nop_out_modules< sgpl_spec_t >( genome.program, should_nop );

  return std::tuple{genome, divergence_updates};

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_BATTLESHIP_NOP_OUT_PHENOTYPICALLY_NEUTRAL_MODULES_HPP_INCLUDE
