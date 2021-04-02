#pragma once
#ifndef DISH2_UTILITY_TRY_WITH_TIMEOUT_HPP_INCLUDE
#define DISH2_UTILITY_TRY_WITH_TIMEOUT_HPP_INCLUDE

#include <future>
#include <mutex>

#include "../config/thread_idx.hpp"

namespace dish2 {

template<typename T, typename Duration>
bool try_with_timeout( T&& task, const Duration& duration ) {

  // setup drawers as passed in args
  const auto parent_thread_idx = dish2::thread_idx;

  // drawings occasionally hang, so add a time out
  std::packaged_task<void()> packaged_task( [&](){
    dish2::thread_idx = parent_thread_idx;
    task();
  } );

  auto future = packaged_task.get_future();
  std::thread worker( std::move(packaged_task) );

  if (future.wait_for( duration ) != std::future_status::timeout) {
    worker.join();
    future.get(); // this will propagate exceptions, if any
    return true;
  } else {
    worker.detach(); // we leave the thread still running
    return false;
  }

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_TRY_WITH_TIMEOUT_HPP_INCLUDE
