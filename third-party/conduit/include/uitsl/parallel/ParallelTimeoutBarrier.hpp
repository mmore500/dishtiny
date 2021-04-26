#pragma once
#ifndef UITSL_PARALLEL_PARALLELTIMEOUTBARRIER_HPP_INCLUDE
#define UITSL_PARALLEL_PARALLELTIMEOUTBARRIER_HPP_INCLUDE

#include <thread>

#include <mpi.h>

#include "../countdown/Timer.hpp"

#include "ThreadIbarrier.hpp"

namespace uitsl {

/**
 * Block until all threads reach the barrier or a timeout is exceeded.
 *
 * @tparam Timer_T class to manage timeout check.
 * @deprecated
 */
template<typename Timer_T=uitsl::Timer<>>
class
#ifndef DOXYGEN_SHOULD_SKIP_THIS
[[deprecated("use ParallelBarrier")]]
#endif
ParallelTimeoutBarrier {

public:

  /**
  * Blocks until all threads have reached barrier, or timer expires.
   */
  ParallelTimeoutBarrier(
    const uitsl::ThreadIbarrier& thread_barrier,
    const Timer_T& timer=Timer_T{}
  ) {

    while (!thread_barrier.IsComplete() && !timer.IsComplete());

  }

};

} // namespace uitsl

#endif // #ifndef UITSL_PARALLEL_PARALLELTIMEOUTBARRIER_HPP_INCLUDE
