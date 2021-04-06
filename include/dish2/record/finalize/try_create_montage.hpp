#pragma once
#ifndef DISH2_RECORD_FINALIZE_TRY_CREATE_MONTAGE_HPP_INCLUDE
#define DISH2_RECORD_FINALIZE_TRY_CREATE_MONTAGE_HPP_INCLUDE

#include <chrono>

#include "../../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"

#include "../../debug/log_msg.hpp"
#include "../../utility/try_with_timeout.hpp"

#include "create_montage.hpp"

namespace dish2 {

void try_create_montage() {

  dish2::log_msg( "trying create_montage with 5 minute timeout" );

  using namespace std::chrono_literals;
  if ( dish2::try_with_timeout( dish2::create_montage, 5min ) ) {
    dish2::log_msg( "created montage" );
  } else {
    dish2::log_msg( "montage creation timed out" );
  }

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_FINALIZE_TRY_CREATE_MONTAGE_HPP_INCLUDE
