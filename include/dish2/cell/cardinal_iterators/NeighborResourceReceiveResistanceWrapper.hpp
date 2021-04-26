#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_NEIGHBORRESOURCERECEIVERESISTANCEWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_NEIGHBORRESOURCERECEIVERESISTANCEWRAPPER_HPP_INCLUDE

#include <type_traits>
#include <utility>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class NeighborResourceReceiveResistanceWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  NeighborResourceReceiveResistanceWrapper() = default;

  NeighborResourceReceiveResistanceWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::ResourceReceiveResistance;
  using pointer = value_type*;
  using reference = value_type&;

  const value_type& operator*() {
    return std::as_const(
      parent_t::operator*().state_node_input
    ).Get().template Get<
      dish2::ResourceReceiveResistance
    >();
  }

  const value_type* operator->() { return &operator*(); }

  NeighborResourceReceiveResistanceWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  NeighborResourceReceiveResistanceWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  NeighborResourceReceiveResistanceWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  NeighborResourceReceiveResistanceWrapper operator--(int) {
    NeighborResourceReceiveResistanceWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  NeighborResourceReceiveResistanceWrapper operator+(const RHS rhs) {
    NeighborResourceReceiveResistanceWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  NeighborResourceReceiveResistanceWrapper operator-(const RHS rhs) {
    NeighborResourceReceiveResistanceWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_NEIGHBORRESOURCERECEIVERESISTANCEWRAPPER_HPP_INCLUDE
