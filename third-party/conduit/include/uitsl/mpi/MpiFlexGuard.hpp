#pragma once
#ifndef UITSL_MPI_MPIFLEXGUARD_HPP_INCLUDE
#define UITSL_MPI_MPIFLEXGUARD_HPP_INCLUDE

#include <variant>

#include "MpiGuard.hpp"
#include "MpiMultithreadGuard.hpp"

namespace uitsl {

class MpiFlexGuard {

  std::variant<
    std::monostate, uitsl::MpiGuard, uitsl::MpiMultithreadGuard
  > guard;

public:

  void InitSingleThread() { guard.emplace< uitsl::MpiGuard >(); }

  void InitMultithread() { guard.emplace< uitsl::MpiMultithreadGuard >(); }

};

} // namespace uitsl

#endif // #ifndef UITSL_MPI_MPIFLEXGUARD_HPP_INCLUDE
