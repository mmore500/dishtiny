#pragma once
#ifndef UITSL_MPI_MPIGUARD_HPP_INCLUDE
#define UITSL_MPI_MPIGUARD_HPP_INCLUDE

#include <functional>

#include <mpi.h>

#include "../debug/err_verify.hpp"

namespace uitsl {

struct MpiGuard {

  #ifndef __EMSCRIPTEN__
  MpiGuard() {
    int argc{};
    uitsl::err_verify( MPI_Init(&argc, nullptr) );
  }

  ~MpiGuard() { uitsl::err_verify( MPI_Finalize() ); }
  #endif // #ifndef __EMSCRIPTEN__

};

} // namespace uitsl

#endif // #ifndef UITSL_MPI_MPIGUARD_HPP_INCLUDE
