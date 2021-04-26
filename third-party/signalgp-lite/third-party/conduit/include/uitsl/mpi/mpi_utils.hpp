#pragma once
#ifndef UITSL_MPI_MPI_UTILS_HPP_INCLUDE
#define UITSL_MPI_MPI_UTILS_HPP_INCLUDE

#include <assert.h>
#include <limits>
#include <numeric>
#include <stddef.h>

#include <mpi.h>

#include "../../../third-party/Empirical/include/emp/base/array.hpp"
#include "../../../third-party/Empirical/include/emp/base/errors.hpp"

#include "../debug/safe_cast.hpp"
#include "../utility/print_utils.hpp"

#include "audited_routines.hpp"
#include "comm_utils.hpp"
#include "group_utils.hpp"
#include "proc_id_t.hpp"
#include "request_utils.hpp"
#include "status_utils.hpp"

namespace uitsl {

inline void mpi_init_multithread(int *argc, char ***argv) {
  int res{};
  UITSL_Init_thread(argc, argv, MPI_THREAD_MULTIPLE, &res);
  if (res < MPI_THREAD_MULTIPLE) emp::NotifyWarning(
    "MPI implementation may not provide full multithread support."
  );
}

inline void mpi_init_multithread() {
  int argc{};
  mpi_init_multithread(&argc, nullptr);
}

inline void mpi_init() {
  int argc{};
  UITSL_Init(&argc, nullptr);
}

} // namespace uitsl

#endif // #ifndef UITSL_MPI_MPI_UTILS_HPP_INCLUDE
