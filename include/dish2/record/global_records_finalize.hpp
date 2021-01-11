#pragma once
#ifndef DISH2_RECORD_GLOBAL_RECORDS_FINALIZE_HPP_INCLUDE
#define DISH2_RECORD_GLOBAL_RECORDS_FINALIZE_HPP_INCLUDE

#include <algorithm>
#include <cstdlib>

#include "../../../third-party/conduit/include/uitsl/debug/err_discard.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/err_verify.hpp"
#include "../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/keyname_directory_filter.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/keyname_directory_max.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"

#include "make_filename/make_montage_filename.hpp"

namespace dish2 {

void create_deduplicated_drawing_archive() {

  uitsl::err_verify( std::system(
    "rdfind -makesymlinks true -makeresultsfile false drawings/"
  ) );

  uitsl::err_verify( std::system(
    "GZIP=-9 tar -czvf drawings.tar.gz drawings"
  ) );

}

void create_montage() {

  const size_t last_update = uitsl::keyname_directory_max(
    "update", "drawings/", uitsl::stoszt
  );

  auto most_recent_drawings = uitsl::keyname_directory_filter(
    "update", emp::to_string( last_update ), "drawings/"
  );
  std::sort(std::begin(most_recent_drawings), std::end(most_recent_drawings));

  const std::string command = (
    std::string{}
    + "montage -verbose -label '%f' -font Helvetica -pointsize 10 -background '#000000' -fill 'gray' -define png:size=500x500 -geometry 500x500+2+2 -auto-orient -crop 500x500+0+0 "
    + emp::join_on( most_recent_drawings, " " )
    + " " + dish2::make_montage_filename( last_update )
  );

  // montage is returning an error code of 1, so don't verify
  uitsl::err_discard( std::system(command.c_str()) );

}

void finalize_drawings() {
  create_deduplicated_drawing_archive();
  create_montage();
}

void finalize_metadata() {
  uitsl::err_verify( std::system(
    "for f in a=*; do"
    "  keyname stash --move $f a criteria morph proc thread ext"
    "; done"
  ) );
}

void global_records_finalize() {
  if ( uitsl::is_root() && cfg.DATA_DUMP() ) {
    finalize_drawings();
    finalize_metadata();
  }

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_GLOBAL_RECORDS_FINALIZE_HPP_INCLUDE
