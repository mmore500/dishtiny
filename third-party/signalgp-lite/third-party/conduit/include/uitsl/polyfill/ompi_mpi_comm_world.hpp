#pragma once
#ifndef UITSL_POLYFILL_OMPI_MPI_COMM_WORLD_HPP_INCLUDE
#define UITSL_POLYFILL_OMPI_MPI_COMM_WORLD_HPP_INCLUDE

#include <mpi.h>

#if defined(__EMSCRIPTEN__) && defined(OPEN_MPI)

  struct ompi_predefined_communicator_t {
    ompi_predefined_communicator_t(const int) {}
  };

  ompi_predefined_communicator_t  ompi_mpi_comm_world = {0};

#endif // #if defined(__EMSCRIPTEN__) && defined(OPEN_MPI)

#endif // #ifndef UITSL_POLYFILL_OMPI_MPI_COMM_WORLD_HPP_INCLUDE
