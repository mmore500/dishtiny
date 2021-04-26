#pragma once
#ifndef UITSL_DISTRIBUTED_IBARRIERREQUEST_HPP_INCLUDE
#define UITSL_DISTRIBUTED_IBARRIERREQUEST_HPP_INCLUDE

#include <mpi.h>

#include "../mpi/audited_routines.hpp"
#include "../mpi/mpi_utils.hpp"

namespace uitsl {

/**
 * Post a MPI Ibarrier and easily test for its completion.
 */
class IbarrierRequest {

  /// Request handle corresponding to Ibarrier call.
  MPI_Request request;

public:

  /**
   * @param comm Set of MPI processes to participate in barrier.
   */
  IbarrierRequest(const MPI_Comm comm=MPI_COMM_WORLD) {
    UITSL_Ibarrier(comm, &request);
  }

  /**
   * Test if all processes have reached barrier.
   *
   * @returns true if all processes have reached barrier.
   */
  bool IsComplete() const { return uitsl::test_completion(request); }

};

} // namespace uitsl

#endif // #ifndef UITSL_DISTRIBUTED_IBARRIERREQUEST_HPP_INCLUDE
