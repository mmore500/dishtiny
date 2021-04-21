#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_NEIGHBOREPOCHWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_NEIGHBOREPOCHWRAPPER_HPP_INCLUDE

#include <type_traits>
#include <utility>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class NeighborEpochWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  NeighborEpochWrapper() = default;

  NeighborEpochWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::Epoch;
  using pointer = value_type*;
  using reference = value_type&;

  const value_type& operator*() {
    return std::as_const(
      parent_t::operator*().state_node_input
    ).Get().template Get<
      dish2::Epoch
    >();
  }

  const value_type* operator->() { return &operator*(); }

  NeighborEpochWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  NeighborEpochWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  NeighborEpochWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  NeighborEpochWrapper operator--(int) {
    NeighborEpochWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  NeighborEpochWrapper operator+(const RHS rhs) {
    NeighborEpochWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  NeighborEpochWrapper operator-(const RHS rhs) {
    NeighborEpochWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_NEIGHBOREPOCHWRAPPER_HPP_INCLUDE
