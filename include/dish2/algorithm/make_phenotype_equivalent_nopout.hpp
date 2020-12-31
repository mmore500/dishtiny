#pragma once
#ifndef DISH2_ALGORITHM_MAKE_PHENOTYPE_EQUIVALENT_NOPOUT_HPP_INCLUDE
#define DISH2_ALGORITHM_MAKE_PHENOTYPE_EQUIVALENT_NOPOUT_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/streamstringify.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../config/TemporaryConfigOverride.hpp"
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
  const dish2::TemporaryConfigOverride n_cells(
    "N_CELLS", cfg.PHENOTYPIC_DIVERGENCE_N_CELLS()
  );
  const dish2::TemporaryConfigOverride weak_scaling(
    "WEAK_SCALING", false
  );
  const dish2::TemporaryConfigOverride n_threads(
    "N_THREADS", 1
  );
  const dish2::TemporaryConfigOverride mutation_rate(
    "MUTATION_RATE", uitsl::stringstreamify( decltype(cfg.MUTATION_RATE()){} )
  );

  emp_assert( std::none_of(
    std::begin( cfg.MUTATION_RATE() ), std::end( cfg.MUTATION_RATE() ),
    std::identity
  ) );

  const dish2::LogScope guard1{ "noping out phenotypically neutral sites" };

  genome = dish2::nop_out_phenotypically_neutral_modules< Spec >( genome );

  genome = dish2::nop_out_phenotypically_neutral_instructions< Spec >( genome );

  return genome;

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_MAKE_PHENOTYPE_EQUIVALENT_NOPOUT_HPP_INCLUDE
