#pragma once
#ifndef DISH2_RECORD_FINALIZE_FINALIZE_ARTIFACTS_HPP_INCLUDE
#define DISH2_RECORD_FINALIZE_FINALIZE_ARTIFACTS_HPP_INCLUDE

#include <cstdlib>
#include <iostream>

#include "../../../../third-party/conduit/include/uitsl/debug/err_verify.hpp"

namespace dish2 {

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

} // namespace dish2

#endif // #ifndef DISH2_RECORD_FINALIZE_FINALIZE_ARTIFACTS_HPP_INCLUDE
