#pragma once
#ifndef DISH2_RECORD_FINALIZE_TRY_ANIMATE_FRAMES_HPP_INCLUDE
#define DISH2_RECORD_FINALIZE_TRY_ANIMATE_FRAMES_HPP_INCLUDE

#include <chrono>

#include "../../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"

#include "../../debug/log_msg.hpp"
#include "../../utility/try_with_timeout.hpp"

#include "animate_frames.hpp"

namespace dish2 {

void try_animate_frames() {

  using namespace std::chrono_literals;

  dish2::log_msg( "trying animate_frames with 10 minute timeout" );
  if ( dish2::try_with_timeout( dish2::animate_frames, 10min ) ) {
    dish2::log_msg( "animate_frames succeeded" );
  } else {
    dish2::log_msg( "animate_frames timed out" );
  }

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_FINALIZE_TRY_ANIMATE_FRAMES_HPP_INCLUDE
