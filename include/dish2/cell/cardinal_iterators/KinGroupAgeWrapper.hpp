#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_KINGROUPAGEWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_KINGROUPAGEWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class KinGroupAgeWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  KinGroupAgeWrapper() = default;

  KinGroupAgeWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::KinGroupAge<Spec>;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().peripheral.readable_state.template Get<
      dish2::KinGroupAge<Spec>
    >();
  }

  value_type* operator->() { return &operator*(); }

  KinGroupAgeWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  KinGroupAgeWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  KinGroupAgeWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  KinGroupAgeWrapper operator--(int) {
    KinGroupAgeWrapper res{ *this };
    res -= 1;
    return res;
  }

  KinGroupAgeWrapper operator+(const size_t rhs) {
    KinGroupAgeWrapper res{ *this };
    res += rhs;
    return res;
  }

  KinGroupAgeWrapper operator-(const size_t rhs) {
    KinGroupAgeWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_KINGROUPAGEWRAPPER_HPP_INCLUDE
