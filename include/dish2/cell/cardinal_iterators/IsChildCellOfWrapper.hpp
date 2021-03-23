#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_ISCHILDCELLOFWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_ISCHILDCELLOFWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class IsChildCellOfWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  IsChildCellOfWrapper() = default;

  IsChildCellOfWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::IsChildCellOf;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().peripheral.readable_state.template Get<
      dish2::IsChildCellOf
    >();
  }

  value_type* operator->() { return &operator*(); }

  IsChildCellOfWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  IsChildCellOfWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  IsChildCellOfWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  IsChildCellOfWrapper operator--(int) {
    IsChildCellOfWrapper res{ *this };
    res -= 1;
    return res;
  }

  IsChildCellOfWrapper operator+(const size_t rhs) {
    IsChildCellOfWrapper res{ *this };
    res += rhs;
    return res;
  }

  IsChildCellOfWrapper operator-(const size_t rhs) {
    IsChildCellOfWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_ISCHILDCELLOFWRAPPER_HPP_INCLUDE
