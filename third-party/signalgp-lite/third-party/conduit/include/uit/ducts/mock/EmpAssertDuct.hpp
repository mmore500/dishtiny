#pragma once
#ifndef UIT_DUCTS_MOCK_EMPASSERTDUCT_HPP_INCLUDE
#define UIT_DUCTS_MOCK_EMPASSERTDUCT_HPP_INCLUDE

#include <stddef.h>
#include <string>

#include "../../../../third-party/Empirical/include/emp/base/assert.hpp"

#include "../proc/impl/backend/MockBackEnd.hpp"

namespace uit {

/**
 * TODO
 *
 * @tparam ImplSpec class with static and typedef members specifying
 * implementation details for the conduit framework.
 */
template<typename ImplSpec>
class EmpAssertDuct {

  using T = typename ImplSpec::T;

public:

  using InletImpl = EmpAssertDuct<ImplSpec>;
  using OutletImpl = EmpAssertDuct<ImplSpec>;
  using BackEndImpl = uit::MockBackEnd<ImplSpec>;

  T val{};

  /**
   * TODO.
   */
  template <typename... Args>
  EmpAssertDuct(Args&&... args) { ; }

  /**
   * TODO.
   *
   * @throws TODO.
   */
  bool TryPut(const T&) const {
    emp_assert(false, "TryPut called on EmpAssertDuct");
    return false;
  }

  /**
   * TODO.
   *
   */
  bool TryFlush() const {
    emp_assert(false, "Flush called on EmpAssertDuct");
    return true;
  }

  /**
   * TODO.
   *
   * @throws TODO.
   */
  const T& Get() const {
    emp_assert(false, "Get called on EmpAssertDuct");
    return val;
  }

  /**
   * TODO.
   *
   * @throws TODO.
   */
  T& Get() {
    emp_assert(false, "Get called on EmpAssertDuct");
    return val;
  }

  /**
   * TODO.
   *
   * @throws TODO.
   */
  size_t TryConsumeGets(size_t) const {
    emp_assert(false, "ConsumeGets called on EmpAssertDuct");
    return 0;
  }

  /**
   * TODO.
   *
   * @returns TODO.
   */
  static std::string GetName() { return "EmpAssertDuct"; }

  static constexpr bool CanStep() { return false; }

  /**
   * TODO.
   *
   * @returns TODO.
   */
  std::string ToString() const { return std::string{}; }

};

} // namespace uit

#endif // #ifndef UIT_DUCTS_MOCK_EMPASSERTDUCT_HPP_INCLUDE
