#pragma once
#ifndef DISH2_RECORD_MAKE_FILENAME_MAKE_COALESCENCE_RESULT_FILENAME_HPP_INCLUDE
#define DISH2_RECORD_MAKE_FILENAME_MAKE_COALESCENCE_RESULT_FILENAME_HPP_INCLUDE

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
#include "../../load/get_innoculum_slugs.hpp"

namespace dish2 {

std::string make_coalescence_result_filename(
  const size_t thread_idx
) {
  auto keyname_attributes = emp::keyname::unpack_t{
    {"a", "coalescence_result"},
    {"proc", emp::to_string( uitsl::get_proc_id() )},
    {"source", EMP_STRINGIFY(DISHTINY_HASH_)},
    {"thread", emp::to_string(thread_idx)},
    {"ext", ".csv"}
  };

  for (const auto& [root_id, slug] : dish2::get_innoculum_slugs()) {
    keyname_attributes[ emp::to_string("root_id:", root_id) ] = slug;
  }

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

#endif // #ifndef DISH2_RECORD_MAKE_FILENAME_MAKE_COALESCENCE_RESULT_FILENAME_HPP_INCLUDE
