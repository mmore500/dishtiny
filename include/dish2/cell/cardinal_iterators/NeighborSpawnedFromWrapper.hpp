#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_NEIGHBORSPAWNEDFROMWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_NEIGHBORSPAWNEDFROMWRAPPER_HPP_INCLUDE

#include <type_traits>
#include <utility>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class NeighborSpawnedFromWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  NeighborSpawnedFromWrapper() = default;

  NeighborSpawnedFromWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::SpawnedFrom;
  using pointer = value_type*;
  using reference = value_type&;

  const value_type& operator*() {
    return std::as_const(
      parent_t::operator*().state_node_input
    ).Get().template Get<
      dish2::SpawnedFrom
    >();
  }

  const value_type* operator->() { return &operator*(); }

  NeighborSpawnedFromWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  NeighborSpawnedFromWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  NeighborSpawnedFromWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  NeighborSpawnedFromWrapper operator--(int) {
    NeighborSpawnedFromWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  NeighborSpawnedFromWrapper operator+(const RHS rhs) {
    NeighborSpawnedFromWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  NeighborSpawnedFromWrapper operator-(const RHS rhs) {
    NeighborSpawnedFromWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_NEIGHBORSPAWNEDFROMWRAPPER_HPP_INCLUDE
