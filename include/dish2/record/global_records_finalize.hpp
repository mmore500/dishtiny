#pragma once
#ifndef DISH2_RECORD_GLOBAL_RECORDS_FINALIZE_HPP_INCLUDE
#define DISH2_RECORD_GLOBAL_RECORDS_FINALIZE_HPP_INCLUDE

#include <algorithm>
#include <cstdlib>
#include <string>

#include "../../../third-party/conduit/include/uitsl/debug/err_discard.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/err_verify.hpp"
#include "../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/audited_routines.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/keyname_directory_filter.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/keyname_directory_max.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "make_filename/make_montage_filename.hpp"

namespace dish2 {

std::string make_drawing_archive_filename() {

  auto keyname_attributes = emp::keyname::unpack_t{
    {"a", "drawings"},
    {"source", EMP_STRINGIFY(DISHTINY_HASH_)},
    {"ext", ".tar.gz"},
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


void create_deduplicated_drawing_archive() {

  uitsl::err_verify( std::system(
    "rdfind -makesymlinks true -makeresultsfile false drawings/"
  ) );

  const std::string zip_command( emp::to_string(
    "GZIP=-9 tar -czf ", make_drawing_archive_filename(), " drawings/"
  ) );
  uitsl::err_verify( std::system( zip_command.c_str() ) );

}

void create_montage() {

  const size_t last_update = uitsl::keyname_directory_max(
    "update",
    {{"proc", "0"}, {"thread", "0"}, {"ext", ".png"}},
    "drawings/",
    uitsl::stoszt
  );

  // use only drawings from thread 0 proc 0
  auto target_drawings = uitsl::keyname_directory_filter(
    {
      {"update", emp::to_string( last_update )},
      {"proc", "0"},
      {"thread", "0"},
      {"ext", ".png"},
    }, "drawings/"
  );
  std::sort(std::begin(target_drawings), std::end(target_drawings));

  // attach label as metadata to source files, which montage will then read in
  // adapted from https://stackoverflow.com/a/32012488
  for (const auto& target : target_drawings) {
    const auto keyname_attrs =  emp::keyname::unpack( target );
    const std::string a = keyname_attrs.at( "a" );
    const std::string idx = keyname_attrs.count( "idx" )
      ? keyname_attrs.at( "idx" )
      : ""
    ;
    const std::string command =
      std::string{} + "mogrify -label '" + a + " " + idx + "' " + target.string();
    uitsl_err_audit( std::system(command.c_str()) );
  }

  const std::string command = (
    std::string{}
    + "montage -verbose -label '%l' -font Helvetica -pointsize 25 -background '#000000' -fill 'gray' -define png:size=500x500 -geometry 500x500+2+2 -auto-orient "
    + emp::join_on( target_drawings, " " )
    + " " + dish2::make_montage_filename( last_update )
    + " || echo \"montage exit code was $?\""
  );

  // montage is returning an error code of 1, so don't verify
  uitsl::err_discard( std::system(command.c_str()) );

}

void finalize_drawings() {
  // cd doesn't propagate out of std::system call
  uitsl::err_verify( std::system(
    "cd drawings && for f in *a=*; do"
    "  keyname stash --move $f a proc thread update stint series ext"
    "; done"
  ) );
  create_deduplicated_drawing_archive();
  create_montage();
}

void finalize_metadata() {
  uitsl::err_verify( std::system(
    "for f in *a=*; do"
    "  keyname stash --move $f a criteria morph proc stint series thread ext"
    "; done"
  ) );
}

void global_records_finalize() {

  UITSL_Barrier( MPI_COMM_WORLD );

  if ( uitsl::is_root() && cfg.DATA_DUMP() ) {
    finalize_drawings();
    finalize_metadata();
  }

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_GLOBAL_RECORDS_FINALIZE_HPP_INCLUDE
