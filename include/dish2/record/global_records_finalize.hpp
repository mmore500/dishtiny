#pragma once
#ifndef DISH2_RECORD_GLOBAL_RECORDS_FINALIZE_HPP_INCLUDE
#define DISH2_RECORD_GLOBAL_RECORDS_FINALIZE_HPP_INCLUDE

#include <algorithm>
#include <cstdlib>
#include <string>

#include "../../../third-party/conduit/include/uitsl/debug/err_discard.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/err_verify.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/list_cwd.hpp"
#include "../../../third-party/conduit/include/uitsl/math/math_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/audited_routines.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"
#include "../../../third-party/conduit/include/uitsl/polyfill/filesystem.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/keyname_directory_filter.hpp"
#include "../../../third-party/conduit/include/uitsl/utility/keyname_directory_max.hpp"
#include "../../../third-party/Empirical/include/emp/base/macros.hpp"
#include "../../../third-party/Empirical/include/emp/tools/keyname_utils.hpp"
#include "../../../third-party/Empirical/include/emp/tools/string_utils.hpp"

#include "create_montage.hpp"
#include "make_filename/make_drawing_archive_filename.hpp"
#include "make_filename/make_drawing_path.hpp"
#include "make_filename/make_montage_filename.hpp"
#include "make_filename/make_zip_path.hpp"
#include "../utility/animate_script.hpp"

namespace dish2 {

void create_deduplicated_drawing_archive() {

  uitsl::err_verify( std::system(
    "rdfind -makesymlinks true -makeresultsfile false outdrawings/"
  ) );
  std::cout << "rdfind deduplication complete" << '\n';

  const std::string zip_command( emp::to_string(
    "tar -cJf ",
    dish2::make_zip_path( dish2::make_drawing_archive_filename() ),
    " outdrawings/"
  ) );
  uitsl::err_verify( std::system( zip_command.c_str() ) );
  std::cout << "drawing archive zip complete" << '\n';

}

void finalize_drawings() {
  std::cout << "finalize_drawings begin" << '\n';
  // cd doesn't propagate out of std::system call
  create_deduplicated_drawing_archive();
  dish2::create_montage();
  uitsl::err_verify( std::system( "bash -c '"
    "shopt -s nullglob; "
    "cd outdrawings && for f in *a=*; do"
    "  keyname stash --move \"${f}\""
    "  a proc replicate thread update stint series treatment variation ext"
    "; done"
  "'" ) );
  std::cout << "finalize_drawings complete" << '\n';
}

void finalize_artifacts() {
  std::cout << "finalize_artifacts begin" << '\n';
  // cd doesn't propagate out of std::system call
  uitsl::err_verify( std::system( "bash -c '"
    "shopt -s nullglob; "
    "cd outartifacts && for f in *a=*; do"
    "  keyname stash --move \"${f}\""
    "    a criteria morph proc stint series thread ext"
    "    nopout_coarseness nopout_target replicate variation root_id"
    "    $(echo \"$f\" | grep -o \"root_id%[[:digit:]]\\+\")"
    "; done"
  "'" ) );
  std::cout << "finalize_artifacts complete" << '\n';
}

void finalize_data() {
  std::cout << "finalize_data begin" << '\n';
  // cd doesn't propagate out of std::system call
  uitsl::err_verify( std::system( "bash -c '"
    "shopt -s nullglob; "
    "cd outdata && for f in *a=*; do"
    "  keyname stash --move \"${f}\""
    "    a criteria morph proc stint series thread ext"
    "    nopout_coarseness nopout_target replicate variation root_id"
    "    $(echo \"$f\" | grep -o \"root_id:[[:digit:]]\\+\")"
    "; done"
  "'" ) );
  std::cout << "finalize_artifacts complete" << '\n';
}

void finalize_zip() {
  std::cout << "finalize_zip begin" << '\n';
  // cd doesn't propagate out of std::system call
  uitsl::err_verify( std::system( "bash -c '"
    "shopt -s nullglob; "
    "cd outzips && for f in *a=*; do"
    "  keyname stash --move \"${f}\""
    "    a proc replicate stint series thread treatment ext"
    "; done"
  "'" ) );
  std::cout << "finalize_zip complete" << '\n';
}

void output_video() {
  const std::string command = emp::to_string(
    "python3 ",
    dish2::py::setup_script_animate(),
    " ",
    dish2::cfg.VIDEO_FPS()
  );
  uitsl::err_verify( std::system( command.c_str() ) );
}

void global_records_finalize() {

  UITSL_Barrier( MPI_COMM_WORLD );

  if ( uitsl::is_root() ) {
    if ( dish2::cfg.OUTPUT_VIDEO() ) output_video();
    if (
      dish2::cfg.ALL_DRAWINGS_WRITE()
      || dish2::cfg.SELECTED_DRAWINGS().size()
    ) finalize_drawings();
    finalize_artifacts();
    finalize_data();
    finalize_zip();

    #ifndef __EMSCRIPTEN__
      // hash all files, excluding source directory
      uitsl::err_verify( std::system( "tree --du -h" ) );
    #else
      uitsl::list_cwd();
    #endif // #ifndef __EMSCRIPTEN__

  }

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_GLOBAL_RECORDS_FINALIZE_HPP_INCLUDE
