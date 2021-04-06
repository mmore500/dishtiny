#pragma once
#ifndef DISH2_RECORD_FINALIZE_FINALIZE_VIDEOS_HPP_INCLUDE
#define DISH2_RECORD_FINALIZE_FINALIZE_VIDEOS_HPP_INCLUDE

#include <cstdlib>

#include "../../../../third-party/conduit/include/uitsl/debug/err_verify.hpp"

#include "../../debug/log_msg.hpp"

namespace dish2 {

void finalize_videos() {
  dish2::log_msg( "finalize_videos begin" );
  // cd doesn't propagate out of std::system call
  uitsl::err_verify( std::system( "bash -c '"
    "shopt -s nullglob; "
    "cd outvideos && for f in *a=*; do"
    "  keyname stash --move \"${f}\""
    "  a proc idx replicate thread update stint series treatment variation ext"
    "; done"
  "'" ) );
  dish2::log_msg( "finalize_videos complete" );
}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_FINALIZE_FINALIZE_VIDEOS_HPP_INCLUDE
