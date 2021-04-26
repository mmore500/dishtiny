#pragma once
#ifndef DISH2_RECORD_FINALIZE_FINALIZE_DRAWINGS_HPP_INCLUDE
#define DISH2_RECORD_FINALIZE_FINALIZE_DRAWINGS_HPP_INCLUDE

#include <cstdlib>

#include "../../../../third-party/conduit/include/uitsl/debug/err_verify.hpp"

#include "../../debug/log_msg.hpp"

#include "create_deduplicated_drawing_archive.hpp"
#include "try_create_montage.hpp"

namespace dish2 {

void finalize_drawings() {
  dish2::log_msg( "finalize_drawings begin" );
  // cd doesn't propagate out of std::system call
  create_deduplicated_drawing_archive();
  dish2::try_create_montage();
  uitsl::err_verify( std::system( "bash -c '"
    "shopt -s nullglob; "
    "cd outdrawings && for f in *a=*; do"
    "  keyname stash --move \"${f}\""
    "  a proc idx replicate thread update stint series treatment variation ext"
    "; done"
  "'" ) );
  dish2::log_msg( "finalize_drawings complete" );
}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_FINALIZE_FINALIZE_DRAWINGS_HPP_INCLUDE
