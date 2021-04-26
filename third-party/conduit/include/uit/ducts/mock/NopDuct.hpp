#pragma once
#ifndef UIT_DUCTS_MOCK_NOPDUCT_HPP_INCLUDE
#define UIT_DUCTS_MOCK_NOPDUCT_HPP_INCLUDE

#include <stddef.h>
#include <string>

#include "../proc/impl/backend/MockBackEnd.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class NopDuct {

  using T = typename ImplSpec::T;

public:

  using InletImpl = NopDuct<ImplSpec>;
  using OutletImpl = NopDuct<ImplSpec>;
  using BackEndImpl = uit::MockBackEnd<ImplSpec>;

  T val{};

  /**
   * TODO.
   */
  template <typename... Args>
  NopDuct(Args&&... args) { ; }

  /**
   * TODO.
   *
   * @param val TODO.
   */
  bool TryPut(const T&) { return false; }

  /**
   * TODO.
   *
   */
  bool TryFlush() const { return true; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  const T& Get() const { return val; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  T& Get() { return val; }

  /**
   * TODO.
   *
   * @return TODO.
   */
  size_t TryConsumeGets(size_t) { return 0; }

  /**
   * TODO.
   */
  static std::string GetName() { return "NopDuct"; }

  static constexpr bool CanStep() { return true; }

  /**
   * TODO.
   */
  std::string ToString() const { return std::string{}; }

};

} // namespace uit

#endif // #ifndef UIT_DUCTS_MOCK_NOPDUCT_HPP_INCLUDE
