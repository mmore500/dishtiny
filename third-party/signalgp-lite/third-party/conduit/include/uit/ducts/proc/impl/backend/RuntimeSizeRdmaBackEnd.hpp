#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_BACKEND_RUNTIMESIZERDMABACKEND_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_BACKEND_RUNTIMESIZERDMABACKEND_HPP_INCLUDE

#include "../../../../../uitsl/distributed/RdmaWindowManager.hpp"

#include "RdmaBackEnd.hpp"
#include "RuntimeSizeBackEnd.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class RuntimeSizeRdmaBackEnd {

  RdmaBackEnd<ImplSpec> rdma;
  RuntimeSizeBackEnd<ImplSpec> rts;

public:

  template<typename ...Args>
  RuntimeSizeRdmaBackEnd(Args&&... args)
  : rts(std::forward<Args>(args)...)
  { ; }

  size_t GetSize() const { return rts.GetSize(); }

  bool HasSize() const { return rts.HasSize(); }

  uitsl::RdmaWindowManager& GetWindowManager() {
    return rdma.GetWindowManager();
  }

  void Initialize() { rdma.Initialize(); rts.Initialize(); }

};

} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_BACKEND_RUNTIMESIZERDMABACKEND_HPP_INCLUDE
