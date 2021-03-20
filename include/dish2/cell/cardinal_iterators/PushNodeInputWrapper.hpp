#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_PUSHNODEINPUTWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_PUSHNODEINPUTWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class PushNodeInputWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  PushNodeInputWrapper() = default;

  PushNodeInputWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = typename Cardinal<Spec>::push_node_input_t;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().push_node_input;
  }

  value_type* operator->() { return &operator*(); }

  PushNodeInputWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  PushNodeInputWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  PushNodeInputWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  PushNodeInputWrapper operator--(int) {
    PushNodeInputWrapper res{ *this };
    res -= 1;
    return res;
  }

  PushNodeInputWrapper operator+(const size_t rhs) {
    PushNodeInputWrapper res{ *this };
    res += rhs;
    return res;
  }

  PushNodeInputWrapper operator-(const size_t rhs) {
    PushNodeInputWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_PUSHNODEINPUTWRAPPER_HPP_INCLUDE
