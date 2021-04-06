#pragma once
#ifndef DISH2_RECORD_FINALIZE_FINALIZE_DATA_HPP_INCLUDE
#define DISH2_RECORD_FINALIZE_FINALIZE_DATA_HPP_INCLUDE

#include <cstdlib>
#include <iostream>

#include "../../../../third-party/conduit/include/uitsl/debug/err_verify.hpp"

#include "../../debug/log_msg.hpp"

namespace dish2 {

void finalize_data() {
  dish2::log_msg("finalize_data begin");
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
  dish2::log_msg("finalize_data complete");
}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_FINALIZE_FINALIZE_DATA_HPP_INCLUDE
