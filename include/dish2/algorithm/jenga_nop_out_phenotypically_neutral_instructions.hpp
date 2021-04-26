#pragma once
#ifndef DISH2_ALGORITHM_JENGA_NOP_OUT_PHENOTYPICALLY_NEUTRAL_INSTRUCTIONS_HPP_INCLUDE
#define DISH2_ALGORITHM_JENGA_NOP_OUT_PHENOTYPICALLY_NEUTRAL_INSTRUCTIONS_HPP_INCLUDE

#include <algorithm>
#include <iostream>
#include <tuple>

#include "../../../third-party/Empirical/include/emp/base/optional.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_op_instructions.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/morph/nop_out_instructions.hpp"

#include "../debug/log_msg.hpp"
#include "../debug/LogScope.hpp"
#include "../genome/Genome.hpp"
#include "../utility/make_targeted_mask.hpp"
#include "../world/ThreadWorld.hpp"

#include "assess_instructions_for_phenotypic_divergence.hpp"

namespace dish2 {

template< typename Spec >
auto try_jenga_nop_out_phenotypically_neutral_instructions(
  const dish2::Genome<Spec>& genome,
  const size_t nop_length,
  const dish2::Genome<Spec>& reference_genome
) {

  using genome_t = dish2::Genome<Spec>;
  using sgpl_spec_t = typename Spec::sgpl_spec_t;

  emp_assert( genome.program.size() == reference_genome.program.size() );

  const dish2::LogScope guard{ "evaluating instruction-by-instruction" };
  dish2::log_msg(
    "evaluating ", sgpl::count_op_instructions<sgpl_spec_t>( genome.program ),
    " op instructions ", nop_length, " at a time"
  );

  constexpr size_t num_tries = 4;

  emp::vector< emp::optional<genome_t> > neutral_masks( num_tries );
  emp::vector< size_t > divergence_updates( num_tries );

  volatile bool success_flag = false;

  #pragma omp parallel for shared(success_flag)
  for (size_t try_ = 0; try_ < num_tries; ++try_) {

    if ( success_flag ) continue;

    #ifdef __EMSCRIPTEN__
    const dish2::LogScope guard3{
      emp::to_string("evaluating instruction set ", try_)
    };
    #endif // #ifdef __EMSCRIPTEN__

    const auto mask = dish2::make_targeted_mask(
      genome.program, nop_length, try_
    );
    const auto res = dish2::assess_instructions_for_phenotypic_divergence<Spec>(
      genome, mask, reference_genome
    );
    divergence_updates[ try_ ] = res;
    const bool is_neutral = (res == cfg.PHENOTYPIC_DIVERGENCE_N_UPDATES());

    if ( is_neutral ) {
      neutral_masks[ try_ ] = genome;
      neutral_masks[ try_ ]->program = sgpl::nop_out_instructions<sgpl_spec_t>(
        genome.program, mask
      );
      success_flag = true;
    }

  }

  dish2::log_msg("evaluation done with success flag ", success_flag);

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
auto jenga_nop_out_phenotypically_neutral_instructions(
  dish2::Genome<Spec> genome,
  const size_t nop_length,
  const dish2::Genome<Spec>& reference_genome
) {

  using sgpl_spec_t = typename Spec::sgpl_spec_t;

  emp_assert( genome.program.size() == reference_genome.program.size() );

  emp::vector< size_t > divergence_updates;

  while ( sgpl::count_op_instructions<sgpl_spec_t>( genome.program ) ) {
    const auto res = try_jenga_nop_out_phenotypically_neutral_instructions(
      genome,
      nop_length,
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

  };

  return std::tuple{
    genome,
    divergence_updates
  };

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_JENGA_NOP_OUT_PHENOTYPICALLY_NEUTRAL_INSTRUCTIONS_HPP_INCLUDE
