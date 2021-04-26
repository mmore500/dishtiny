#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_PUSHNODEOUTPUTWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_PUSHNODEOUTPUTWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class PushNodeOutputWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  PushNodeOutputWrapper() = default;

  PushNodeOutputWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = typename Cardinal<Spec>::push_node_output_t;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().push_node_output;
  }

  value_type* operator->() { return &operator*(); }

  PushNodeOutputWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  PushNodeOutputWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  PushNodeOutputWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  PushNodeOutputWrapper operator--(int) {
    PushNodeOutputWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  PushNodeOutputWrapper operator+(const RHS rhs) {
    PushNodeOutputWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  PushNodeOutputWrapper operator-(const RHS rhs) {
    PushNodeOutputWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_PUSHNODEOUTPUTWRAPPER_HPP_INCLUDE
