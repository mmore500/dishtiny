#pragma once
#ifndef DISH2_RECORD_MAKE_DUMP_ABUNDANCE_GENOME_FILENAME_HPP_INCLUDE
#define DISH2_RECORD_MAKE_DUMP_ABUNDANCE_GENOME_FILENAME_HPP_INCLUDE

#include <cstdlib>
#include <limits>
#include <string>

#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../config/cfg.hpp"
#include "../introspection/get_endeavor.hpp"

namespace dish2 {

std::string make_dump_abundance_genome_filename(
  const size_t thread_idx,
  const double abundance,
  const std::string& morph
) {

  auto keyname_attributes = emp::keyname::unpack_t{
    {"a", "genome"},
    {"criteria", "abundance"},
    {"abundance", emp::to_string( abundance )},
    {"morph", morph},
    {"source", EMP_STRINGIFY(DISHTINY_HASH_)},
    {"thread", emp::to_string(thread_idx)},
    {"ext", ".json"}
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

  if ( cfg.REPLICATE() != std::numeric_limits<size_t>::max() ) {
    keyname_attributes[ "replicate" ] = emp::to_string( cfg.REPLICATE() );
  }

  if ( dish2::get_endeavor() ) {
    keyname_attributes[ "endeavor" ] = *dish2::get_endeavor();
  }

  return emp::keyname::pack( keyname_attributes );
}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_MAKE_DUMP_ABUNDANCE_GENOME_FILENAME_HPP_INCLUDE
