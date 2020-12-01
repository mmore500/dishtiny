#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_HEIRREQUESTWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_HEIRREQUESTWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class HeirRequestWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  HeirRequestWrapper() = default;

  HeirRequestWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::HeirRequest;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().peripheral.readable_state.template Get<
      dish2::HeirRequest
    >();
  }

  value_type* operator->() { return &operator*(); }

  HeirRequestWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  HeirRequestWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  HeirRequestWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  HeirRequestWrapper operator--(int) {
    HeirRequestWrapper res{ *this };
    res -= 1;
    return res;
  }

  HeirRequestWrapper operator+(const size_t rhs) {
    HeirRequestWrapper res{ *this };
    res += rhs;
    return res;
  }

  HeirRequestWrapper operator-(const size_t rhs) {
    HeirRequestWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_HEIRREQUESTWRAPPER_HPP_INCLUDE
