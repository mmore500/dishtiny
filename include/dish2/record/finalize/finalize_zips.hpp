#pragma once
#ifndef DISH2_RECORD_FINALIZE_FINALIZE_ZIPS_HPP_INCLUDE
#define DISH2_RECORD_FINALIZE_FINALIZE_ZIPS_HPP_INCLUDE

#include <cstdlib>

#include "../../../../third-party/conduit/include/uitsl/debug/err_verify.hpp"

#include "../../debug/log_msg.hpp"

namespace dish2 {

void finalize_zips() {
  dish2::log_msg( "finalize_zips begin" );
  // cd doesn't propagate out of std::system call
  uitsl::err_verify( std::system( "bash -c '"
    "shopt -s nullglob; "
    "cd outzips && for f in *a=*; do"
    "  keyname stash --move \"${f}\""
    "    a proc replicate stint series thread treatment ext"
    "; done"
  "'" ) );
  dish2::log_msg( "finalize_zips complete" );
}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_FINALIZE_FINALIZE_ZIPS_HPP_INCLUDE
