#pragma once
#ifndef UIT_DUCTS_PROC_IMPL_BACKEND_RUNTIMESIZEBACKEND_HPP_INCLUDE
#define UIT_DUCTS_PROC_IMPL_BACKEND_RUNTIMESIZEBACKEND_HPP_INCLUDE

#include <limits>

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class RuntimeSizeBackEnd {

  size_t size;

public:

  RuntimeSizeBackEnd(
    const size_t size_=std::numeric_limits<size_t>::max()
  ) : size(size_)
  { ; }

  void Initialize() { ; }

  size_t GetSize() const { return size; }

  bool HasSize() const { return size != std::numeric_limits<size_t>::max(); }

};

} // namespace uit

#endif // #ifndef UIT_DUCTS_PROC_IMPL_BACKEND_RUNTIMESIZEBACKEND_HPP_INCLUDE
