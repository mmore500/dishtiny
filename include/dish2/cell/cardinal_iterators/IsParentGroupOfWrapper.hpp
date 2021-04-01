#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_ISPARENTGROUPOFWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_ISPARENTGROUPOFWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class IsParentGroupOfWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  IsParentGroupOfWrapper() = default;

  IsParentGroupOfWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::IsParentGroupOf<Spec>;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().peripheral.readable_state.template Get<
      dish2::IsParentGroupOf<Spec>
    >();
  }

  value_type* operator->() { return &operator*(); }

  IsParentGroupOfWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  IsParentGroupOfWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  IsParentGroupOfWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  IsParentGroupOfWrapper operator--(int) {
    IsParentGroupOfWrapper res{ *this };
    res -= 1;
    return res;
  }

  IsParentGroupOfWrapper operator+(const size_t rhs) {
    IsParentGroupOfWrapper res{ *this };
    res += rhs;
    return res;
  }

  IsParentGroupOfWrapper operator-(const size_t rhs) {
    IsParentGroupOfWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_ISPARENTGROUPOFWRAPPER_HPP_INCLUDE
