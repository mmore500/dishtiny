#pragma once
#ifndef DISH2_RECORD_WRITE_DRAWINGS_HPP_INCLUDE
#define DISH2_RECORD_WRITE_DRAWINGS_HPP_INCLUDE

#include <chrono>
#include <future>
#include <mutex>
#include <utility>

#include "../config/thread_idx.hpp"
#include "../world/ThreadWorld.hpp"

#include "drawings/DrawerCollection.hpp"

namespace dish2 {

template< typename Spec >
void write_drawings( const dish2::ThreadWorld< Spec >& thread_world ) {

  dish2::DrawerCollection<Spec> drawers( thread_world );

  const auto parent_thread_idx = dish2::thread_idx;

  // drawings occasionally hang, so add a time out
  std::packaged_task<void()> task( [&](){
    dish2::thread_idx = parent_thread_idx;
    drawers.SaveToFile();
  } );

  auto future = task.get_future();
  std::thread worker( std::move(task) );

  using namespace std::chrono_literals;
  if (future.wait_for( 5min ) != std::future_status::timeout) {
    worker.join();
    future.get(); // this will propagate exception if any
  } else {
    worker.detach(); // we leave the thread still running
    std::cout << "proc " << uitsl::get_proc_id()
      << " thread " << dish2::thread_idx
      << " drawings write timed out" << '\n';
  }

  thread_local std::once_flag once_flag;
  std::call_once(once_flag, [](){

    std::cout << "proc " << uitsl::get_proc_id()
      << " thread " << dish2::thread_idx
      << " wrote drawings" << '\n';
  });

}

} // namespace dish2

#endif // #ifndef DISH2_RECORD_WRITE_DRAWINGS_HPP_INCLUDE
