#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_NEIGHBORNUMKNOWNQUORUMBITSWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_NEIGHBORNUMKNOWNQUORUMBITSWRAPPER_HPP_INCLUDE

#include <type_traits>
#include <utility>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class NeighborNumKnownQuorumBitsWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  NeighborNumKnownQuorumBitsWrapper() = default;

  NeighborNumKnownQuorumBitsWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::NumKnownQuorumBits<Spec>;
  using pointer = value_type*;
  using reference = value_type&;

  const value_type& operator*() {
    return std::as_const(
      parent_t::operator*().state_node_input
    ).Get().template Get<
      dish2::NumKnownQuorumBits<Spec>
    >();
  }

  const value_type* operator->() { return &operator*(); }

  NeighborNumKnownQuorumBitsWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  NeighborNumKnownQuorumBitsWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  NeighborNumKnownQuorumBitsWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  NeighborNumKnownQuorumBitsWrapper operator--(int) {
    NeighborNumKnownQuorumBitsWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  NeighborNumKnownQuorumBitsWrapper operator+(const RHS rhs) {
    NeighborNumKnownQuorumBitsWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  NeighborNumKnownQuorumBitsWrapper operator-(const RHS rhs) {
    NeighborNumKnownQuorumBitsWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_NEIGHBORNUMKNOWNQUORUMBITSWRAPPER_HPP_INCLUDE
