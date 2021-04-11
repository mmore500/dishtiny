#pragma once
#ifndef DISH2_UTILITY_TRY_WITH_TIMEOUT_HPP_INCLUDE
#define DISH2_UTILITY_TRY_WITH_TIMEOUT_HPP_INCLUDE

#include <future>
#include <mutex>

#include <pthread.h>

#include "../config/thread_idx.hpp"
#include "../debug/log_msg.hpp"

namespace dish2 {

template<typename T, typename Duration>
bool try_with_timeout( T&& task, const Duration& duration ) {

  const auto parent_thread_idx = dish2::thread_idx;

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

    dish2::log_msg(
      "try_with_timeout exceeded, "
      "detaching and cancelling worker thread"
    );
    const auto handle = worker.native_handle();
    worker.detach();
    dish2::log_msg( "worker thread detached" );
    pthread_cancel( handle );
    dish2::log_msg( "worker thread canceled" );
    dish2::log_msg( "try_with_timeout complete" );

    return false;
  }

}

} // namespace dish2

#endif // #ifndef DISH2_UTILITY_TRY_WITH_TIMEOUT_HPP_INCLUDE
