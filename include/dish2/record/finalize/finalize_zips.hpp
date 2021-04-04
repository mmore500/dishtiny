#pragma once
#ifndef DISH2_RECORD_FINALIZE_FINALIZE_ZIPS_HPP_INCLUDE
#define DISH2_RECORD_FINALIZE_FINALIZE_ZIPS_HPP_INCLUDE

#include <cstdlib>
#include <iostream>

#include "../../../../third-party/conduit/include/uitsl/debug/err_verify.hpp"

namespace dish2 {

void finalize_zips() {
  std::cout << "finalize_zips begin" << '\n';
  // cd doesn't propagate out of std::system call
  uitsl::err_verify( std::system( "bash -c '"
    "shopt -s nullglob; "
    "cd outzips && for f in *a=*; do"
    "  keyname stash --move \"${f}\""
    "    a proc replicate stint series thread treatment ext"
    "; done"
  "'" ) );
  std::cout << "finalize_zips complete" << '\n';
}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_FINALIZE_FINALIZE_ZIPS_HPP_INCLUDE
