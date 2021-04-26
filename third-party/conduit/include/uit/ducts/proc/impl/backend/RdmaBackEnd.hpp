#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_BACKEND_RDMABACKEND_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_BACKEND_RDMABACKEND_HPP_INCLUDE

#include "../../../../../uitsl/distributed/RdmaWindowManager.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class RdmaBackEnd {

  uitsl::RdmaWindowManager window_manager;

public:

  uitsl::RdmaWindowManager& GetWindowManager() {
    return window_manager;
  }

  void Initialize() { window_manager.Initialize(); }

};

} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_BACKEND_RDMABACKEND_HPP_INCLUDE
