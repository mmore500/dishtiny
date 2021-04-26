#pragma once
#ifndef DISH2_RECORD_MAKE_FILENAME_MAKE_BATTLESHIP_DIVERGENCE_UPDATES_FILENAME_HPP_INCLUDE
#define DISH2_RECORD_MAKE_FILENAME_MAKE_BATTLESHIP_DIVERGENCE_UPDATES_FILENAME_HPP_INCLUDE

#include <cstdlib>
#include <string>

#include "../../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/base/macros.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../config/cfg.hpp"
#include "../../config/get_endeavor.hpp"
#include "../../config/get_repro.hpp"
#include "../../config/get_slurm_job_id.hpp"
#include "../../config/has_replicate.hpp"
#include "../../config/has_series.hpp"
#include "../../config/has_stint.hpp"

namespace dish2 {

std::string make_battleship_divergence_updates_filename(
  const std::string& criteria,
  const size_t nopout_coarseness,
  const std::string& nopout_target
) {

  auto keyname_attributes = emp::keyname::unpack_t{
    {"a", "battleship_nopout_divergence_updates"},
    {
      "num_evaluated_updates",
      emp::to_string(cfg.PHENOTYPIC_DIVERGENCE_N_UPDATES())
    },
    {"criteria", criteria},
    {"nopout_coarseness", emp::to_string( nopout_coarseness )},
    {"nopout_target", nopout_target},
    {"source", EMP_STRINGIFY(DISHTINY_HASH_)},
    {"treatment", emp::keyname::demote( dish2::cfg.TREATMENT() )},
    {"ext", ".txt"}
  };

  if ( dish2::get_repro() ) {
    keyname_attributes[ "_repro" ] = *dish2::get_repro();
  }

  if ( dish2::has_series() ) {
    keyname_attributes[ "series" ] = emp::to_string( cfg.SERIES() );
  }

  if ( dish2::has_stint() ) {
    keyname_attributes[ "stint" ] = emp::to_string( cfg.STINT() );
  }

  if ( dish2::has_replicate() ) {
    keyname_attributes[ "replicate" ] = cfg.REPLICATE();
  }

  if ( dish2::get_endeavor() ) {
    keyname_attributes[ "_endeavor" ] = emp::to_string(*dish2::get_endeavor());
  }

  if ( dish2::get_slurm_job_id() ) {
    keyname_attributes[ "_slurm_job_id" ] = emp::to_string(
      *dish2::get_slurm_job_id()
    );
  }

  return emp::keyname::pack( keyname_attributes );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_MAKE_FILENAME_MAKE_BATTLESHIP_DIVERGENCE_UPDATES_FILENAME_HPP_INCLUDE
