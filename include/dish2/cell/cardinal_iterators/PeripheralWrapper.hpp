#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_PERIPHERALWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_PERIPHERALWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class PeripheralWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  PeripheralWrapper() = default;

  PeripheralWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = typename Cardinal<Spec>::peripheral_t;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() { return parent_t::operator*().peripheral; }

  value_type* operator->() { return &operator*(); }

  PeripheralWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  PeripheralWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  PeripheralWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  PeripheralWrapper operator--(int) {
    PeripheralWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  PeripheralWrapper operator+(const RHS rhs) {
    PeripheralWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  PeripheralWrapper operator-(const RHS rhs) {
    PeripheralWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_PERIPHERALWRAPPER_HPP_INCLUDE
