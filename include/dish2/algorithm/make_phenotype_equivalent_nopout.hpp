#pragma once
#ifndef DISH2_ALGORITHM_MAKE_PHENOTYPE_EQUIVALENT_NOPOUT_HPP_INCLUDE
#define DISH2_ALGORITHM_MAKE_PHENOTYPE_EQUIVALENT_NOPOUT_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/streamstringify.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../debug/entry_types.hpp"
#include "../debug/log_event.hpp"
#include "../debug/LogScope.hpp"
#include "../genome/Genome.hpp"

#include "nop_out_phenotypically_neutral_instructions.hpp"
#include "nop_out_phenotypically_neutral_modules.hpp"

namespace dish2 {

template< typename Spec >
dish2::Genome<Spec> make_phenotype_equivalent_nopout(
  dish2::Genome<Spec> genome
) {

  // store old config values, temporarily overwrite them
  const auto n_cells_bak = cfg.N_CELLS();
  const auto weak_scaling_bak = cfg.WEAK_SCALING();
  const auto n_threads_bak = cfg.N_THREADS();
  const auto mutation_rate_bak = cfg.MUTATION_RATE();

  cfg.Set( "N_CELLS", emp::to_string(cfg.PHENOTYPIC_DIVERGENCE_N_CELLS()) );
  cfg.Set( "WEAK_SCALING", "0" );
  cfg.Set( "N_THREADS", "1" );
  cfg.Set(
    "MUTATION_RATE", uitsl::stringstreamify( decltype(cfg.MUTATION_RATE()){} )
  );

  emp_assert( std::none_of(
    std::begin( cfg.MUTATION_RATE() ), std::end( cfg.MUTATION_RATE() ),
    std::identity
  ) );

  const dish2::LogScope guard1{ "noping out phenotypically neutral sites" };

  genome = dish2::nop_out_phenotypically_neutral_modules< Spec >( genome );

  genome = dish2::nop_out_phenotypically_neutral_instructions< Spec >( genome );

  // restore old config values
  cfg.Set( "N_CELLS", emp::to_string(n_cells_bak) );
  cfg.Set( "WEAK_SCALING", emp::to_string(weak_scaling_bak) );
  cfg.Set( "N_THREADS", emp::to_string(n_threads_bak) );
  cfg.Set( "MUTATION_RATE", uitsl::stringstreamify(mutation_rate_bak) );

  emp_assert( cfg.MUTATION_RATE() == mutation_rate_bak );

  return genome;

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_MAKE_PHENOTYPE_EQUIVALENT_NOPOUT_HPP_INCLUDE
