#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_OPTIMUMQUORUMEXCEEDEDWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_OPTIMUMQUORUMEXCEEDEDWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class OptimumQuorumExceededWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  OptimumQuorumExceededWrapper() = default;

  OptimumQuorumExceededWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::OptimumQuorumExceeded<Spec>;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().peripheral.readable_state.template Get<
      dish2::OptimumQuorumExceeded<Spec>
    >();
  }

  value_type* operator->() { return &operator*(); }

  OptimumQuorumExceededWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  OptimumQuorumExceededWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  OptimumQuorumExceededWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  OptimumQuorumExceededWrapper operator--(int) {
    OptimumQuorumExceededWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  OptimumQuorumExceededWrapper operator+(const RHS rhs) {
    OptimumQuorumExceededWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  OptimumQuorumExceededWrapper operator-(const RHS rhs) {
    OptimumQuorumExceededWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_OPTIMUMQUORUMEXCEEDEDWRAPPER_HPP_INCLUDE
