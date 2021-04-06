#pragma once
#ifndef DISH2_RECORD_FINALIZE_CREATE_MONTAGE_HPP_INCLUDE
#define DISH2_RECORD_FINALIZE_CREATE_MONTAGE_HPP_INCLUDE

#include <algorithm>
#include <cstdlib>
#include <string>

#include "../../../../third-party/conduit/include/uitsl/debug/err_discard.hpp"
#include "../../../../third-party/conduit/include/uitsl/debug/err_verify.hpp"
#include "../../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../../third-party/conduit/include/uitsl/mpi/audited_routines.hpp"
#include "../../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../../third-party/conduit/include/uitsl/utility/keyname_directory_filter.hpp"
#include "../../../../third-party/conduit/include/uitsl/utility/keyname_directory_max.hpp"
#include "../../../../third-party/Empirical/include/emp/base/macros.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "../../debug/log_msg.hpp"

#include "../make_filename/make_drawing_path.hpp"
#include "../make_filename/make_montage_filename.hpp"

namespace dish2 {

void create_montage() {

  if ( uitsl::keyname_directory_filter(
    {{"proc", "0"}, {"thread", "0"}, {"ext", ".png"}},
    "outdrawings/"
  ).empty() ) {
    dish2::log_msg(
      "proc ", uitsl::get_proc_id()," no drawings to create montage, skipping"
    );
    return;
  }

  const size_t last_update = uitsl::keyname_directory_max(
    "update",
    {{"proc", "0"}, {"thread", "0"}, {"ext", ".png"}},
    "outdrawings/",
    uitsl::stoszt
  );

  // use only drawings from thread 0 proc 0
  auto target_drawings = uitsl::keyname_directory_filter(
    {
      {"update", emp::to_string( last_update )},
      {"proc", "0"},
      {"thread", "0"},
      {"ext", ".png"},
    }, "outdrawings/"
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
    + "montage -verbose -label '%l' -font 'Liberation-Mono' -pointsize 20 -background 'black' -fill 'white' -define png:size=500x500 -geometry 500x500+2+2 -auto-orient "
    + emp::join_on( target_drawings, " " )
    + " "
    + dish2::make_drawing_path(dish2::make_montage_filename( last_update ))
    + " || echo \"montage exit code was $?\""
  );

  // montage is returning an error code of 1, so don't verify
  uitsl::err_discard( std::system(command.c_str()) );

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_FINALIZE_CREATE_MONTAGE_HPP_INCLUDE
