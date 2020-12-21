#pragma once
#ifndef DISH2_RECORD_MAKE_PHYLOGENETIC_ROOT_ABUNDANCES_FILENAME_HPP_INCLUDE
#define DISH2_RECORD_MAKE_PHYLOGENETIC_ROOT_ABUNDANCES_FILENAME_HPP_INCLUDE

#include <cstdlib>
#include <limits>
#include <string>

#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#include "../config/cfg.hpp"

namespace dish2 {

std::string make_phylogenetic_root_abundances_filename(
  const size_t thread_idx
) {
  auto keyname_attributes = emp::keyname::unpack_t{
    {"a", "phylogenetic_root_abundances"},
    {"source", EMP_STRINGIFY(DISHTINY_HASH_)},
    {"thread", emp::to_string(thread_idx)},
    {"ext", ".csv"}
  };

  if ( std::getenv("REPRO_ID") ) {
    keyname_attributes[ "repro" ] = std::getenv("REPRO_ID");
  }

  if ( cfg.SERIES() != std::numeric_limits<size_t>::max() ) {
    keyname_attributes[ "series" ] = emp::to_string( cfg.SERIES() );
  }

  if ( cfg.STINT() != std::numeric_limits<size_t>::max() ) {
    keyname_attributes[ "stint" ] = emp::to_string( cfg.STINT() );
  }

  return emp::keyname::pack( keyname_attributes );
}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_MAKE_PHYLOGENETIC_ROOT_ABUNDANCES_FILENAME_HPP_INCLUDE
