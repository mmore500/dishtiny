#pragma once
#ifndef DISH2_RECORD_FINALIZE_FINALIZE_DRAWINGS_HPP_INCLUDE
#define DISH2_RECORD_FINALIZE_FINALIZE_DRAWINGS_HPP_INCLUDE

#include <cstdlib>
#include <iostream>

#include "../../../../third-party/conduit/include/uitsl/debug/err_verify.hpp"

#include "create_deduplicated_drawing_archive.hpp"
#include "try_create_montage.hpp"

namespace dish2 {

void finalize_drawings() {
  std::cout << "finalize_drawings begin" << '\n';
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
  std::cout << "finalize_drawings complete" << '\n';
}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_FINALIZE_FINALIZE_DRAWINGS_HPP_INCLUDE
