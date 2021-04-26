#pragma once
#ifndef UITSL_DISTRIBUTED_DISTRIBUTEDTIMEOUTBARRIER_HPP_INCLUDE
#define UITSL_DISTRIBUTED_DISTRIBUTEDTIMEOUTBARRIER_HPP_INCLUDE

#include <thread>

#include <mpi.h>

#include "../countdown/Timer.hpp"
#include "../mpi/mpi_utils.hpp"
#include "../parallel/ThreadIbarrier.hpp"

#include "IbarrierRequest.hpp"

namespace uitsl {

/**
 * Block until all processes reach the barrier or a timeout is exceeded.
 *
 * @tparam Timer_T class to manage timeout check.
 */
// TODO is Ibarrier request leaked?
template<typename Timer_T=uitsl::Timer<>>
class DistributedTimeoutBarrier {

  /// manages state of MPI Ibarrier call
  uitsl::IbarrierRequest proc_barrier;

public:

  /**
  * Blocks until all processes have reached barrier or a timeout is exceeded.
   */
  DistributedTimeoutBarrier(
    const Timer_T& timer=Timer_T{},
    const MPI_Comm comm=MPI_COMM_WORLD
  ) : proc_barrier(comm) {

    while (!proc_barrier.IsComplete() && !timer.IsComplete());

  }

};

} // namespace uitsl

#endif // #ifndef UITSL_DISTRIBUTED_DISTRIBUTEDTIMEOUTBARRIER_HPP_INCLUDE
