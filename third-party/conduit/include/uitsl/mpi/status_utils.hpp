#pragma once
#ifndef UITSL_MPI_STATUS_UTILS_HPP_INCLUDE
#define UITSL_MPI_STATUS_UTILS_HPP_INCLUDE

#include <sstream>
#include <string>

#include <mpi.h>

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"

#include "../utility/print_utils.hpp"

#include "audited_routines.hpp"
#include "name_mpi_error.hpp"

namespace uitsl {

inline int get_count(const MPI_Status& status, const MPI_Datatype& datatype) {
  int res;
  UITSL_Get_count(
    &status, // const MPI_Status * status: return status of receive operation
    datatype, // MPI_Datatype datatype: datatype of each receive buffer element
    &res // int *count: number of received elements (integer)
  );
  emp_assert( res != MPI_UNDEFINED );
  return res;
}

inline bool test_cancelled(const MPI_Status& status) {
  int res;
  UITSL_Test_cancelled(&status, &res);
  return res;
}

inline std::string to_string(const MPI_Status& status) {
  std::stringstream ss;
  ss << uitsl::format_member(
    "MPI_Get_count",
    uitsl::get_count(status, MPI_BYTE)
  ) << '\n';
  ss << uitsl::format_member(
    "MPI_Test_cancelled",
    uitsl::test_cancelled(status)
  ) << '\n';
  ss << uitsl::format_member(
    "int MPI_SOURCE",
    (int) status.MPI_SOURCE
  ) << '\n';
  ss << uitsl::format_member(
    "int MPI_TAG",
    (int) status.MPI_TAG
  ) << '\n';
  ss << uitsl::format_member(
    "int MPI_ERROR",
    (int) status.MPI_ERROR
  ) << '\n';
  ss << uitsl::format_member(
    "error enum",
    uitsl::name_mpi_error(status.MPI_ERROR)
  );
  return ss.str();
}

} // namespace uitsl

#endif // #ifndef UITSL_MPI_STATUS_UTILS_HPP_INCLUDE
