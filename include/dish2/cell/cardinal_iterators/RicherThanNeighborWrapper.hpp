#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_RICHERTHANNEIGHBORWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_RICHERTHANNEIGHBORWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class RicherThanNeighborWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  RicherThanNeighborWrapper() = default;

  RicherThanNeighborWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::RicherThanNeighbor;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().peripheral.readable_state.template Get<
      dish2::RicherThanNeighbor
    >();
  }

  value_type* operator->() { return &operator*(); }

  RicherThanNeighborWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  RicherThanNeighborWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  RicherThanNeighborWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  RicherThanNeighborWrapper operator--(int) {
    RicherThanNeighborWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  RicherThanNeighborWrapper operator+(const RHS rhs) {
    RicherThanNeighborWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  RicherThanNeighborWrapper operator-(const RHS rhs) {
    RicherThanNeighborWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_RICHERTHANNEIGHBORWRAPPER_HPP_INCLUDE
