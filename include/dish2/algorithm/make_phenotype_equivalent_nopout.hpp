#pragma once
#ifndef DISH2_ALGORITHM_MAKE_PHENOTYPE_EQUIVALENT_NOPOUT_HPP_INCLUDE
#define DISH2_ALGORITHM_MAKE_PHENOTYPE_EQUIVALENT_NOPOUT_HPP_INCLUDE

#include <algorithm>
#include <fstream>
#include <iostream>

#include "../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/streamstringify.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_instructions.hpp"

#include "../config/TemporaryConfigOverride.hpp"
#include "../debug/entry_types.hpp"
#include "../debug/log_event.hpp"
#include "../debug/LogScope.hpp"
#include "../genome/Genome.hpp"
#include "../record/make_filename/make_data_path.hpp"
#include "../record/make_filename/make_divergence_updates_filename.hpp"

#include "nop_out_phenotypically_neutral_instructions.hpp"
#include "nop_out_phenotypically_neutral_modules.hpp"

namespace dish2 {

template< typename Spec >
dish2::Genome<Spec> make_phenotype_equivalent_nopout(
  dish2::Genome<Spec> genome, const std::string& criteria=""
) {

  // store old config values, temporarily overwrite them
  const dish2::TemporaryConfigOverride n_cells(
    "N_CELLS", cfg.PHENOTYPIC_DIVERGENCE_N_CELLS()
  );
  const dish2::TemporaryConfigOverride weak_scaling(
    "WEAK_SCALING", false
  );
  const dish2::TemporaryConfigOverride diversity_maintentance(
    "DIVERSITY_MAINTENANCE_SERVICE_FREQUENCY", 0
  );
  const dish2::TemporaryConfigOverride stint_diversity_maintentance(
    "STINT_DIVERSITY_MAINTENANCE_SERVICE_FREQUENCY", 0
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

  {
    const auto [nopped_genome, divergence_updates]
      = dish2::nop_out_phenotypically_neutral_modules< Spec >( genome );

    genome = nopped_genome;

    const auto out_path = dish2::make_data_path(
      dish2::make_divergence_updates_filename( criteria, 1, "module" )
    );

    std::cout << "dumping " << divergence_updates.size()
      << " divergence updates for modulewise neutral nopout w/ coarseness 1 to "
      << out_path  << '\n';

    std::ofstream os( out_path );
    for (const auto& upd : divergence_updates) os << upd << '\n';
  }

  // nop out in increasingly fine-grained segments
  // idea is that failing (detecting divergence) tends to be fast
  // so coarse grained searches can give us a big potential speedup
  // at low cost
  {
    const auto [nopped_genome, divergence_updates]
      = dish2::nop_out_phenotypically_neutral_instructions< Spec >(genome, 8);

    genome = nopped_genome;

    const auto out_path = dish2::make_data_path(
      dish2::make_divergence_updates_filename( criteria, 8, "inst" )
    );

    std::cout << "dumping " << divergence_updates.size()
      << " divergence updates for instwise neutral nopout w/ coarseness 8 to "
      << out_path << '\n';

    std::ofstream os( out_path );
    for (const auto& upd : divergence_updates) os << upd << '\n';
  }

  {
    const auto [nopped_genome, divergence_updates]
      = dish2::nop_out_phenotypically_neutral_instructions< Spec >(genome, 4);

    genome = nopped_genome;

    const auto out_path = dish2::make_data_path(
      dish2::make_divergence_updates_filename( criteria, 4, "inst" )
    );

    std::cout << "dumping " << divergence_updates.size()
      << " divergence updates for instwise neutral nopout w/ coarseness 4 to "
      << out_path << '\n';

    std::ofstream os( out_path );
    for (const auto& upd : divergence_updates) os << upd << '\n';
  }

  {
    const auto [nopped_genome, divergence_updates]
      = dish2::nop_out_phenotypically_neutral_instructions< Spec >(genome, 2);

    genome = nopped_genome;

    const auto out_path = dish2::make_data_path(
      dish2::make_divergence_updates_filename( criteria, 2, "inst" )
    );

    std::cout << "dumping " << divergence_updates.size()
      << " divergence updates for instwise neutral nopout w/ coarseness 2 to "
      << out_path << '\n';

    std::ofstream os( out_path );
    for (const auto& upd : divergence_updates) os << upd << '\n';
  }

  {
    const auto [nopped_genome, divergence_updates]
      = dish2::nop_out_phenotypically_neutral_instructions< Spec >(genome, 1);

    genome = nopped_genome;

    const auto out_path = dish2::make_data_path(
      dish2::make_divergence_updates_filename( criteria, 1, "inst" )
    );

    std::cout << "dumping " << divergence_updates.size()
      << " divergence updates for instwise neutral nopout w/ coarseness 1 to "
      << out_path << '\n';

    std::ofstream os( out_path );
    for (const auto& upd : divergence_updates) os << upd << '\n';
  }

  std::cout << sgpl::count_instructions( genome.program, "op" );
  std::cout << " op instructions remain in phenotype equivalent nopout";
  std::cout << '\n';

  return genome;

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_MAKE_PHENOTYPE_EQUIVALENT_NOPOUT_HPP_INCLUDE
