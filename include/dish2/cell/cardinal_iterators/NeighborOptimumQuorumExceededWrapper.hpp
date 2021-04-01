#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_NEIGHBOROPTIMUMQUORUMEXCEEDEDWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_NEIGHBOROPTIMUMQUORUMEXCEEDEDWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class NeighborOptimumQuorumExceededWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  NeighborOptimumQuorumExceededWrapper() = default;

  NeighborOptimumQuorumExceededWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::NeighborOptimumQuorumExceeded<Spec>;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().peripheral.readable_state.template Get<
      dish2::NeighborOptimumQuorumExceeded<Spec>
    >();
  }

  value_type* operator->() { return &operator*(); }

  NeighborOptimumQuorumExceededWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  NeighborOptimumQuorumExceededWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  NeighborOptimumQuorumExceededWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  NeighborOptimumQuorumExceededWrapper operator--(int) {
    NeighborOptimumQuorumExceededWrapper res{ *this };
    res -= 1;
    return res;
  }

  NeighborOptimumQuorumExceededWrapper operator+(const size_t rhs) {
    NeighborOptimumQuorumExceededWrapper res{ *this };
    res += rhs;
    return res;
  }

  NeighborOptimumQuorumExceededWrapper operator-(const size_t rhs) {
    NeighborOptimumQuorumExceededWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_NEIGHBOROPTIMUMQUORUMEXCEEDEDWRAPPER_HPP_INCLUDE
