#pragma once
#ifndef UITSL_PARALLEL_PARALLELBARRIER_HPP_INCLUDE
#define UITSL_PARALLEL_PARALLELBARRIER_HPP_INCLUDE

#include <atomic>

#include "../countdown/Timer.hpp"

#include "ThreadIbarrier.hpp"

namespace uitsl {

/**
 * Block until all threads reach the barrier or a timeout is exceeded.
 *
 * Adapted from https://stackoverflow.com/a/24777186.
 *
 * @tparam Timer_T class to manage timeout check.
 */
class ParallelBarrier {

  size_t expected;
  std::atomic<size_t> missing;
  std::atomic<size_t> generation{};

public:

  ParallelBarrier(
    const size_t expected_
  ) : expected(expected_), missing(expected_) {

  }

  void ArriveAndWait() {
    ArriveAndWaitWhile( uitsl::CoarseTimer{ uitsl::infinite_duration } );
  }

  /**
  * Blocks until all threads have called WaitFor, or timer expires.
   */
  template<typename Timer>
  void ArriveAndWaitWhile(const Timer& timer) {

    const size_t my_generation = generation;

    if (!--missing) {
      missing = expected;
      ++generation;
    } else while (generation == my_generation && !timer.IsComplete());

  }

};

} // namespace uitsl

#endif // #ifndef UITSL_PARALLEL_PARALLELBARRIER_HPP_INCLUDE
