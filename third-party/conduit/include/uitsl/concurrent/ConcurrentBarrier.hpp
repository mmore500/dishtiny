#pragma once
#ifndef UITSL_CONCURRENT_CONCURRENTBARRIER_HPP_INCLUDE
#define UITSL_CONCURRENT_CONCURRENTBARRIER_HPP_INCLUDE

#include <atomic>

#include <mpi.h>

#include "../countdown/Timer.hpp"
#include "../distributed/DistributedTimeoutBarrier.hpp"

namespace uitsl {

/**
 * Block until all threads reach the barrier or a timeout is exceeded.
 *
 * Adapted from https://stackoverflow.com/a/24777186.
 *
 * @tparam Timer_T class to manage timeout check.
 */
class ConcurrentBarrier {

  size_t expected_threads;
  std::atomic<size_t> missing_threads;
  std::atomic<size_t> generation{};

  MPI_Comm comm;

public:

  ConcurrentBarrier(
    const size_t expected_threads,
    const MPI_Comm& comm_=MPI_COMM_WORLD
  ) : expected_threads(expected_threads)
  , missing_threads(expected_threads)
  , comm(comm_) { }

  void ArriveAndWait() {
    ArriveAndWaitWhile( uitsl::CoarseTimer{ uitsl::infinite_duration } );
  }

  /**
  * Blocks until all threads have called WaitFor, or timer expires.
   */
  template<typename Timer>
  void ArriveAndWaitWhile(const Timer& timer) {

    const size_t my_generation = generation;

    if (!--missing_threads) {
      uitsl::DistributedTimeoutBarrier<Timer> barrier( timer, comm );
      missing_threads = expected_threads;
      ++generation;
    } else while (generation == my_generation && !timer.IsComplete());

  }

};

} // namespace uitsl

#endif // #ifndef UITSL_CONCURRENT_CONCURRENTBARRIER_HPP_INCLUDE
