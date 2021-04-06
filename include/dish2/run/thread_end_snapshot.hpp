#pragma once
#ifndef DISH2_RUN_THREAD_END_SNAPSHOT_HPP_INCLUDE
#define DISH2_RUN_THREAD_END_SNAPSHOT_HPP_INCLUDE

#include <iostream>

#include "../../../third-party/conduit/include/uitsl/mpi/comm_utils.hpp"

#include "../config/thread_idx.hpp"
#include "../debug/log_msg.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

template<typename Spec>
void thread_end_snapshot( dish2::ThreadWorld<Spec>& thread_world ) {

  dish2::thread_data_dump<Spec>( thread_world );
  dish2::log_msg( "data dump complete" );

  dish2::thread_data_write<Spec>( thread_world );
  dish2::log_msg( "write 0 complete" );

  thread_world.Update();

  dish2::log_msg( "update step" );
  dish2::thread_data_write<Spec>( thread_world );

  dish2::log_msg( "write 1 complete" );

}

} // namespace dish2

#endif // #ifndef DISH2_RUN_THREAD_END_SNAPSHOT_HPP_INCLUDE
