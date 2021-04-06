#pragma once
#ifndef DISH2_ALGORITHM_EXHAUSTIVE_JENGA_NOP_OUT_PHENOTYPICALLY_NEUTRAL_INSTRUCTIONS_HPP_INCLUDE
#define DISH2_ALGORITHM_EXHAUSTIVE_JENGA_NOP_OUT_PHENOTYPICALLY_NEUTRAL_INSTRUCTIONS_HPP_INCLUDE

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <tuple>

#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_op_instructions.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/morph/nop_out_instructions.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/utility/ThreadLocalRandom.hpp"

#include "../debug/log_msg.hpp"
#include "../debug/LogScope.hpp"
#include "../genome/Genome.hpp"
#include "../record/dump_in_progress_jenga_nopout_genome.hpp"
#include "../record/finalize/finalize_artifacts.hpp"
#include "../utility/static_coarse_timer.hpp"
#include "../world/ThreadWorld.hpp"

#include "assess_instructions_for_phenotypic_divergence.hpp"

namespace dish2 {

template< typename Spec >
auto exhaustive_try_jenga_nop_out_phenotypically_neutral_instructions(
  const dish2::Genome<Spec>& genome,
  const dish2::Genome<Spec>& reference_genome
) {

  using genome_t = dish2::Genome<Spec>;
  using sgpl_spec_t = typename Spec::sgpl_spec_t;

  emp_assert( genome.program.size() == reference_genome.program.size() );

  const dish2::LogScope guard{ "evaluating instruction-by-instruction" };
  dish2::log_msg(
    "exhaustively evaluating ",
    sgpl::count_op_instructions<sgpl_spec_t>( genome.program ),
    " op instructions "
  );

  const size_t num_sites = sgpl::count_op_instructions<sgpl_spec_t>(
    genome.program
  );

  emp::vector< emp::optional<genome_t> > neutral_masks( num_sites );
  emp::vector< size_t > divergence_updates( num_sites );

  volatile bool success_flag = false;

  // so that we don't always start at the same target site and start out going
  // thru what we've already seen... for sake of efficiency (not correctness)
  const size_t offset = sgpl::tlrand.Get().GetUInt( num_sites );

  #pragma omp parallel for shared(success_flag)
  for (size_t site = 0; site < num_sites; ++site) {

    const size_t target = ( offset + site ) % num_sites;

    if ( success_flag ) continue;

    #ifdef __EMSCRIPTEN__
    const dish2::LogScope guard3{
      emp::to_string("evaluating instruction ", target)
    };
    #endif // #ifdef __EMSCRIPTEN__

    genome_t test_genome = genome;
    test_genome.program = sgpl::nop_out_nth_op<sgpl_spec_t>(
      genome.program, target
    );
    const auto res = dish2::run_until_phenotypic_divergence<Spec>(
      test_genome, reference_genome
    );
    divergence_updates[ target ] = res;
    const bool is_neutral = (res == cfg.PHENOTYPIC_DIVERGENCE_N_UPDATES());

    if ( is_neutral ) {
      neutral_masks[ target ] = test_genome;
      success_flag = true;
    }

  }

  dish2::log_msg(
    " exhaustive evaluation done with success_flag ", success_flag
  );

  const auto successful_mask_it = std::find_if(
    std::begin( neutral_masks ),
    std::end( neutral_masks ),
    []( const auto& mask ){ return mask.has_value(); }
  );

  const bool no_successful_mask = successful_mask_it == neutral_masks.end();

  emp_assert( no_successful_mask == !success_flag );

  return std::tuple{
    no_successful_mask ? emp::optional<genome_t>{} : *successful_mask_it,
    divergence_updates
  };

}

template< typename Spec >
auto exhaustive_jenga_nop_out_phenotypically_neutral_instructions(
  dish2::Genome<Spec> genome,
  const dish2::Genome<Spec>& reference_genome
) {

  using sgpl_spec_t = typename Spec::sgpl_spec_t;

  emp_assert( genome.program.size() == reference_genome.program.size() );

  emp::vector< size_t > divergence_updates;

  while ( sgpl::count_op_instructions<sgpl_spec_t>( genome.program ) ) {

    const auto res = exhaustive_try_jenga_nop_out_phenotypically_neutral_instructions(
      genome,
      reference_genome
    );
    const auto& [ neutral_nopped_genome, cur_divergence_updates ] = res;

    divergence_updates.insert(
      std::end( divergence_updates ),
      std::begin( cur_divergence_updates ), std::end( cur_divergence_updates )
    );

    if ( neutral_nopped_genome.has_value() ) {
      genome = *neutral_nopped_genome;
    } else break;

    dish2::log_msg(
      sgpl::count_op_instructions( genome.program ),
      " op instructions remain in phenotype equivalent nopout"
    );

    if (
      dish2::static_coarse_timer.GetElapsed()
      >= std::chrono::duration<double>(
        dish2::cfg.JENGA_NOP_OUT_SAVE_PROGRESS_AND_QUIT_SECONDS()
      )
    ) {

      dish2::log_msg("jenga nopout timeout, saving progress and quitting");

      dish2::dump_in_progress_jenga_nopout_genome<Spec>( genome );
      dish2::finalize_artifacts();

      dish2::log_msg("in progress genome dump complete");

      std::exit( 0 );
    }

  };

  return std::tuple{
    genome,
    divergence_updates
  };

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_EXHAUSTIVE_JENGA_NOP_OUT_PHENOTYPICALLY_NEUTRAL_INSTRUCTIONS_HPP_INCLUDE
