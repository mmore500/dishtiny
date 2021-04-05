#pragma once
#ifndef DISH2_RUN_THREAD_END_SNAPSHOT_HPP_INCLUDE
#define DISH2_RUN_THREAD_END_SNAPSHOT_HPP_INCLUDE

#include <iostream>

#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"

#include "../config/thread_idx.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template<typename Spec>
void thread_end_snapshot( dish2::ThreadWorld<Spec>& thread_world ) {

  dish2::thread_data_dump<Spec>( thread_world );
  std::cout << "proc " << uitsl::get_proc_id()
    << " thread " << dish2::thread_idx
    << " data dump complete" << '\n';

  dish2::thread_data_write<Spec>( thread_world );
  std::cout << "proc " << uitsl::get_proc_id()
    << " thread " << dish2::thread_idx
    << " write 0" << '\n';
  thread_world.Update();
  std::cout << "proc " << uitsl::get_proc_id()
    << " thread " << dish2::thread_idx
    << " update step" << '\n';
  dish2::thread_data_write<Spec>( thread_world );
  std::cout << "proc " << uitsl::get_proc_id()
    << " thread " << dish2::thread_idx
    << " write 1" << '\n';

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_THREAD_END_SNAPSHOT_HPP_INCLUDE
