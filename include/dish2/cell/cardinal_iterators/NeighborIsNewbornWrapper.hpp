#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_NEIGHBORISNEWBORNWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_NEIGHBORISNEWBORNWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class NeighborIsNewbornWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  NeighborIsNewbornWrapper() = default;

  NeighborIsNewbornWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::NeighborIsNewborn;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().peripheral.readable_state.template Get<
      dish2::NeighborIsNewborn
    >();
  }

  value_type* operator->() { return &operator*(); }

  NeighborIsNewbornWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  NeighborIsNewbornWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  NeighborIsNewbornWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  NeighborIsNewbornWrapper operator--(int) {
    NeighborIsNewbornWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  NeighborIsNewbornWrapper operator+(const RHS rhs) {
    NeighborIsNewbornWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  NeighborIsNewbornWrapper operator-(const RHS rhs) {
    NeighborIsNewbornWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_NEIGHBORISNEWBORNWRAPPER_HPP_INCLUDE
