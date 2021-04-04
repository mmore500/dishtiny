#pragma once
#ifndef DISH2_RECORD_FINALIZE_TRY_ANIMATE_FRAMES_HPP_INCLUDE
#define DISH2_RECORD_FINALIZE_TRY_ANIMATE_FRAMES_HPP_INCLUDE

#include <chrono>
#include <iostream>

#include "../../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"

#include "../../utility/try_with_timeout.hpp"

#include "animate_frames.hpp"

namespace dish2 {

void try_animate_frames() {

  using namespace std::chrono_literals;

  if ( dish2::try_with_timeout( dish2::animate_frames, 10min ) ) {
    std::cout << "proc " << uitsl::get_proc_id()
      << " animate_frames succeeded" << '\n';
  } else {
    std::cout << "proc " << uitsl::get_proc_id()
      << " animate_frames timed out" << '\n';
  }

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_FINALIZE_TRY_ANIMATE_FRAMES_HPP_INCLUDE
