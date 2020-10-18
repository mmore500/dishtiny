#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_ISALIVEWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_ISALIVEWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/source/base/vector.h"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class IsAliveWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  IsAliveWrapper() = default;

  IsAliveWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::IsAlive;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().peripheral.readable_state.template Get<
      dish2::IsAlive
    >();
  }

  value_type* operator->() { return &operator*(); }

  IsAliveWrapper operator+(const size_t rhs) {
    IsAliveWrapper res{ *this };
    res += rhs;
    return res;
  }

  IsAliveWrapper operator-(const size_t rhs) {
    IsAliveWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_ISALIVEWRAPPER_HPP_INCLUDE
