#pragma once
#ifndef DISH2_RECORD_MAKE_FILENAME_MAKE_DIVERGENCE_UPDATES_FILENAME_HPP_INCLUDE
#define DISH2_RECORD_MAKE_FILENAME_MAKE_DIVERGENCE_UPDATES_FILENAME_HPP_INCLUDE

#include <cstdlib>
#include <limits>
#include <string>

#include "../../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../config/cfg.hpp"
#include "../../introspection/get_endeavor.hpp"

namespace dish2 {

std::string make_divergence_updates_filename(
  const std::string& criteria,
  const size_t nopout_coarseness,
  const std::string& nopout_target
) {

  auto keyname_attributes = emp::keyname::unpack_t{
    {"a", "nopout_divergence_updates"},
    {
      "num_evaluated_updates",
      emp::to_string(cfg.PHENOTYPIC_DIVERGENCE_N_UPDATES())
    },
    {"criteria", criteria},
    {"nopout_coarseness", emp::to_string( nopout_coarseness )},
    {"nopout_target", nopout_target},
    {"source", EMP_STRINGIFY(DISHTINY_HASH_)},
    {"ext", ".txt"}
  };

  if ( std::getenv("REPRO_ID") ) {
    keyname_attributes[ "repro" ] = std::getenv("REPRO_ID");
  }

  if ( cfg.SERIES() != std::numeric_limits<uint32_t>::max() ) {
    keyname_attributes[ "series" ] = emp::to_string( cfg.SERIES() );
  }

  if ( cfg.STINT() != std::numeric_limits<uint32_t>::max() ) {
    keyname_attributes[ "stint" ] = emp::to_string( cfg.STINT() );
  }

  if ( cfg.REPLICATE() != std::numeric_limits<uint32_t>::max() ) {
    keyname_attributes[ "replicate" ] = emp::to_string( cfg.REPLICATE() );
  }

  if ( dish2::get_endeavor() ) {
    keyname_attributes[ "endeavor" ] = emp::to_string( *dish2::get_endeavor() );
  }

  return emp::keyname::pack( keyname_attributes );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_MAKE_FILENAME_MAKE_DIVERGENCE_UPDATES_FILENAME_HPP_INCLUDE
