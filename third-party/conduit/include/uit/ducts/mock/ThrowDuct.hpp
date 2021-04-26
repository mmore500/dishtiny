#pragma once
#ifndef UIT_DUCTS_MOCK_THROWDUCT_HPP_INCLUDE
#define UIT_DUCTS_MOCK_THROWDUCT_HPP_INCLUDE

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
class ThrowDuct {

  using T = typename ImplSpec::T;

public:

  using InletImpl = ThrowDuct<ImplSpec>;
  using OutletImpl = ThrowDuct<ImplSpec>;
  using BackEndImpl = uit::MockBackEnd<ImplSpec>;

  /**
   * TODO.
   */
  template <typename... Args>
  ThrowDuct(Args&&... args) { ; }

  /**
   * TODO.
   *
   * @throws TODO.
   */
  [[noreturn]] bool TryPut(const T&) const {
    throw "TryPut called on ThrowDuct";
  }

  /**
   * TODO.
   *
   */
  [[noreturn]] bool TryFlush() const { throw "Flush called on ThrowDuct"; }

  /**
  * TODO.
  *
  * @throws TODO.
   */
  [[noreturn]] const T& Get() const { throw "Get called on ThrowDuct"; }

  /**
  * TODO.
  *
  * @throws TODO.
   */
  [[noreturn]] T& Get() { throw "Get called on ThrowDuct"; }

  /**
   * TODO.
   *
   * @throws TODO.
   */
  [[noreturn]] size_t TryConsumeGets(size_t) const {
    throw "ConsumeGets called on ThrowDuct";
  }

  /**
   * TODO.
   */
  static std::string GetName() { return "ThrowDuct"; }

  static constexpr bool CanStep() { return false; }

  /**
   * TODO.
   */
  std::string ToString() const { return std::string{}; }

};

} // namespace uit

#endif // #ifndef UIT_DUCTS_MOCK_THROWDUCT_HPP_INCLUDE
