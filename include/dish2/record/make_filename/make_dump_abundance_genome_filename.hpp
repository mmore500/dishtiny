#pragma once
#ifndef DISH2_RECORD_MAKE_FILENAME_MAKE_DUMP_ABUNDANCE_GENOME_FILENAME_HPP_INCLUDE
#define DISH2_RECORD_MAKE_FILENAME_MAKE_DUMP_ABUNDANCE_GENOME_FILENAME_HPP_INCLUDE

#include <cstdlib>
#include <string>

#include "../../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/base/macros.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../config/cfg.hpp"
#include "../../config/get_endeavor.hpp"
#include "../../config/get_repro.hpp"
#include "../../config/has_replicate.hpp"
#include "../../config/has_series.hpp"
#include "../../config/has_stint.hpp"

namespace dish2 {

std::string make_dump_abundance_genome_filename(
  const size_t thread_idx,
  const size_t count,
  const double abundance,
  const double prevalence,
  const std::string& morph
) {

  auto keyname_attributes = emp::keyname::unpack_t{
    {"a", "genome"},
    {"criteria", "abundance"},
    {"count", emp::to_string( count )},
    {"abundance", emp::to_string( abundance )},
    {"prevalence", emp::to_string( prevalence )},
    {"morph", morph},
    {"variation", "master"},
    {"proc", emp::to_string( uitsl::get_proc_id() )},
    {"source", EMP_STRINGIFY(DISHTINY_HASH_)},
    {"thread", emp::to_string(thread_idx)},
    {"treatment", emp::keyname::demote( dish2::cfg.TREATMENT() )},
    {"ext", ".json.gz"}
  };

  if ( dish2::get_repro() ) {
    keyname_attributes[ "repro" ] = *dish2::get_repro();
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
    keyname_attributes[ "endeavor" ] = emp::to_string( *dish2::get_endeavor() );
  }

  return emp::keyname::pack( keyname_attributes );
}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_MAKE_FILENAME_MAKE_DUMP_ABUNDANCE_GENOME_FILENAME_HPP_INCLUDE
