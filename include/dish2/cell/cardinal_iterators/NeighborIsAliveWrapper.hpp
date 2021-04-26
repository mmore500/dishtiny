#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_NEIGHBORISALIVEWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_NEIGHBORISALIVEWRAPPER_HPP_INCLUDE

#include <type_traits>
#include <utility>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class NeighborIsAliveWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  NeighborIsAliveWrapper() = default;

  NeighborIsAliveWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::IsAlive;
  using pointer = value_type*;
  using reference = value_type&;

  const value_type& operator*() {
    return std::as_const(
      parent_t::operator*().state_node_input
    ).Get().template Get<
      dish2::IsAlive
    >();
  }

  const value_type* operator->() { return &operator*(); }

  NeighborIsAliveWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  NeighborIsAliveWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  NeighborIsAliveWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  NeighborIsAliveWrapper operator--(int) {
    NeighborIsAliveWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  NeighborIsAliveWrapper operator+(const RHS rhs) {
    NeighborIsAliveWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  NeighborIsAliveWrapper operator-(const RHS rhs) {
    NeighborIsAliveWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_NEIGHBORISALIVEWRAPPER_HPP_INCLUDE
