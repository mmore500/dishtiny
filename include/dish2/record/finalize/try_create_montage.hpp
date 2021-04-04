#pragma once
#ifndef DISH2_RECORD_FINALIZE_TRY_CREATE_MONTAGE_HPP_INCLUDE
#define DISH2_RECORD_FINALIZE_TRY_CREATE_MONTAGE_HPP_INCLUDE

#include <chrono>

#include "../../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"

#include "../../utility/try_with_timeout.hpp"

#include "create_montage.hpp"

namespace dish2 {

void try_create_montage() {

  using namespace std::chrono_literals;
  if ( dish2::try_with_timeout( dish2::create_montage, 5min ) ) {
    std::cout << "proc " << uitsl::get_proc_id()
      << " created montage" << '\n';
  } else {
    std::cout << "proc " << uitsl::get_proc_id()
      << " montage creation timed out" << '\n';
  }

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_FINALIZE_TRY_CREATE_MONTAGE_HPP_INCLUDE
