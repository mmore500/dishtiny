#pragma once
#ifndef DISH2_RECORD_GLOBAL_RECORDS_FINALIZE_HPP_INCLUDE
#define DISH2_RECORD_GLOBAL_RECORDS_FINALIZE_HPP_INCLUDE

#include <cstdlib>

#include "../../../third-party/conduit/include/uitsl/debug/err_verify.hpp"
#include "../../../third-party/conduit/include/uitsl/debug/list_cwd.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/audited_routines.hpp"
#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"

#include "finalize/finalize_artifacts.hpp"
#include "finalize/finalize_benchmarks.hpp"
#include "finalize/finalize_data.hpp"
#include "finalize/finalize_drawings.hpp"
#include "finalize/finalize_videos.hpp"
#include "finalize/finalize_zips.hpp"
#include "finalize/try_animate_frames.hpp"
#include "finalize/try_create_montage.hpp"

namespace dish2 {

void global_records_finalize() {

  UITSL_Barrier( MPI_COMM_WORLD );

  if ( uitsl::is_root() ) {
    if ( dish2::cfg.ANIMATE_FRAMES() ) try_animate_frames();
    if (
      dish2::cfg.ALL_DRAWINGS_WRITE()
      || dish2::cfg.SELECTED_DRAWINGS().size()
    ) finalize_drawings();
    finalize_artifacts();
    finalize_benchmarks();
    finalize_data();
    finalize_videos();
    finalize_zips();

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
