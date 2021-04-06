#pragma once
#ifndef DISH2_ALGORITHM_MAKE_JENGA_PHENOTYPE_EQUIVALENT_NOPOUT_HPP_INCLUDE
#define DISH2_ALGORITHM_MAKE_JENGA_PHENOTYPE_EQUIVALENT_NOPOUT_HPP_INCLUDE

#include <algorithm>
#include <fstream>
#include <iostream>

#include "../../../third-party/conduit/include/uitsl/polyfill/identity.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/streamstringify.hpp"
#include "../../../third-party/Empirical/include/emp/base/vector.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_op_instructions.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/morph/nop_out_nth_op.hpp"

#include "../config/TemporaryConfigOverride.hpp"
#include "../debug/entry_types.hpp"
#include "../debug/log_event.hpp"
#include "../debug/log_msg.hpp"
#include "../debug/LogScope.hpp"
#include "../genome/Genome.hpp"
#include "../record/make_filename/make_data_path.hpp"
#include "../record/make_filename/make_jenga_divergence_updates_filename.hpp"
#include "../utility/pare_keyname_filename.hpp"

#include "exhaustive_jenga_nop_out_phenotypically_neutral_instructions.hpp"
#include "jenga_nop_out_phenotypically_neutral_instructions.hpp"

namespace dish2 {

template< typename Spec >
dish2::Genome<Spec> make_jenga_phenotype_equivalent_nopout(
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

  const dish2::LogScope guard1{
    "jenga noping out phenotypically neutral sites"
  };

  const dish2::Genome<Spec> reference_genome = genome;

  // nop out in increasingly fine-grained segments
  // idea is that failing (detecting divergence) tends to be fast
  // so coarse grained searches can give us a big potential speedup
  // at low cost
  for (
    size_t coarseness = reference_genome.program.size() / 2;
    coarseness > 1;
    coarseness /= 2
  ) {
    const auto [nopped_genome, divergence_updates]
      = dish2::jenga_nop_out_phenotypically_neutral_instructions< Spec >(genome, coarseness, reference_genome);

    genome = nopped_genome;

    const auto out_filename = dish2::pare_keyname_filename(
      dish2::make_jenga_divergence_updates_filename(
        criteria, coarseness, "inst"
      ),
      dish2::make_data_path()
    );
    const auto out_path = dish2::make_data_path( out_filename );

    dish2::log_msg(
      sgpl::count_op_instructions( genome.program ),
      " op instructions remain in phenotype equivalent nopout"
    );

    dish2::log_msg(
      "dumping ", divergence_updates.size(),
      " divergence updates for instwise neutral nopout w/ coarseness ",
      coarseness, " to ", out_path
    );

    std::ofstream os( out_path, std::ios_base::app );
    for (const auto& upd : divergence_updates) os << upd << '\n';
  }

  // final nop out should be one by one

  {
    const auto [nopped_genome, divergence_updates]
      = dish2::exhaustive_jenga_nop_out_phenotypically_neutral_instructions<
        Spec
      >( genome, reference_genome );

    genome = nopped_genome;

    const auto out_filename = dish2::pare_keyname_filename(
      dish2::make_jenga_divergence_updates_filename( criteria, 1, "inst" ),
      dish2::make_data_path()
    );
    const auto out_path = dish2::make_data_path( out_filename );

    dish2::log_msg(
      sgpl::count_op_instructions( genome.program ),
      " op instructions remain in phenotype equivalent nopout"
    );
    dish2::log_msg(
      "dumping ", divergence_updates.size(),
      " divergence updates for instwise neutral nopout w/ coarseness 1 to ",
      out_path
    );

    std::ofstream os( out_path, std::ios_base::app );
    for (const auto& upd : divergence_updates) os << upd << '\n';
  }

  dish2::log_msg(
    sgpl::count_op_instructions( genome.program ),
    " op instructions remain in phenotype equivalent nopout"
  );

  return genome;

}

} // namespace dish2

#endif // #ifndef DISH2_ALGORITHM_MAKE_JENGA_PHENOTYPE_EQUIVALENT_NOPOUT_HPP_INCLUDE
