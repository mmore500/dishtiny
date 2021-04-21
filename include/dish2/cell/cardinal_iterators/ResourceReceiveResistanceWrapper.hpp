#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_RESOURCERECEIVERESISTANCEWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_RESOURCERECEIVERESISTANCEWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class ResourceReceiveResistanceWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  ResourceReceiveResistanceWrapper() = default;

  ResourceReceiveResistanceWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::ResourceReceiveResistance;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().peripheral.readable_state.template Get<
      dish2::ResourceReceiveResistance
    >();
  }

  value_type* operator->() { return &operator*(); }

  ResourceReceiveResistanceWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  ResourceReceiveResistanceWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  ResourceReceiveResistanceWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  ResourceReceiveResistanceWrapper operator--(int) {
    ResourceReceiveResistanceWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  ResourceReceiveResistanceWrapper operator+(const RHS rhs) {
    ResourceReceiveResistanceWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  ResourceReceiveResistanceWrapper operator-(const RHS rhs) {
    ResourceReceiveResistanceWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_RESOURCERECEIVERESISTANCEWRAPPER_HPP_INCLUDE
