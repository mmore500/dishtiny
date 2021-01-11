#pragma once
#ifndef DISH2_RECORD_MAKE_FILENAME_MAKE_DRAWING_FILENAME_HPP_INCLUDE
#define DISH2_RECORD_MAKE_FILENAME_MAKE_DRAWING_FILENAME_HPP_INCLUDE

#include <cstdlib>
#include <limits>
#include <string>

#include "../../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../config/cfg.hpp"
#include "../../introspection/get_endeavor.hpp"

namespace dish2 {

std::string make_drawing_filename(
  const size_t series_idx,
  const size_t thread_idx,
  const size_t update,
  const std::string& artist_slug
) {

  auto keyname_attributes = emp::keyname::unpack_t{
    {"a", artist_slug},
    {"idx", emp::to_string( series_idx )},
    {"proc", emp::to_string( uitsl::get_proc_id() )},
    {"source", EMP_STRINGIFY(DISHTINY_HASH_)},
    {"thread", emp::to_string(thread_idx)},
    {"update", emp::to_string(update)},
    {"ext", ".png"}
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
    keyname_attributes[ "endeavor" ] = *dish2::get_endeavor();
  }

  return "drawings/" + emp::keyname::pack( keyname_attributes );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_MAKE_FILENAME_MAKE_DRAWING_FILENAME_HPP_INCLUDE
