#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_STATENODEINPUTWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_STATENODEINPUTWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class StateNodeInputWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  StateNodeInputWrapper() = default;

  StateNodeInputWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = typename Cardinal<Spec>::state_node_input_t;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().state_node_input;
  }

  value_type* operator->() { return &operator*(); }

  StateNodeInputWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  StateNodeInputWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  StateNodeInputWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  StateNodeInputWrapper operator--(int) {
    StateNodeInputWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  StateNodeInputWrapper operator+(const RHS rhs) {
    StateNodeInputWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  StateNodeInputWrapper operator-(const RHS rhs) {
    StateNodeInputWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_STATENODEINPUTWRAPPER_HPP_INCLUDE
