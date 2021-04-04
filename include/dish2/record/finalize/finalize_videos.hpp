#pragma once
#ifndef DISH2_RECORD_FINALIZE_FINALIZE_VIDEOS_HPP_INCLUDE
#define DISH2_RECORD_FINALIZE_FINALIZE_VIDEOS_HPP_INCLUDE

#include <cstdlib>
#include <iostream>

#include "../../../../third-party/conduit/include/uitsl/debug/err_verify.hpp"

namespace dish2 {

void finalize_videos() {
  std::cout << "finalize_videos begin" << '\n';
  // cd doesn't propagate out of std::system call
  uitsl::err_verify( std::system( "bash -c '"
    "shopt -s nullglob; "
    "cd outvideos && for f in *a=*; do"
    "  keyname stash --move \"${f}\""
    "  a proc idx replicate thread update stint series treatment variation ext"
    "; done"
  "'" ) );
  std::cout << "finalize_videos complete" << '\n';
}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_FINALIZE_FINALIZE_VIDEOS_HPP_INCLUDE
