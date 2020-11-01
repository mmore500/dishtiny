#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_CELLAGEWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_CELLAGEWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/source/base/vector.h"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class CellAgeWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  CellAgeWrapper() = default;

  CellAgeWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::CellAge;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().peripheral.readable_state.template Get<
      dish2::CellAge
    >();
  }

  value_type* operator->() { return &operator*(); }

  CellAgeWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  CellAgeWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  CellAgeWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  CellAgeWrapper operator--(int) {
    CellAgeWrapper res{ *this };
    res -= 1;
    return res;
  }

  CellAgeWrapper operator+(const size_t rhs) {
    CellAgeWrapper res{ *this };
    res += rhs;
    return res;
  }

  CellAgeWrapper operator-(const size_t rhs) {
    CellAgeWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_CELLAGEWRAPPER_HPP_INCLUDE
