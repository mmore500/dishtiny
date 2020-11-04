#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_READABLESTATEWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_READABLESTATEWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/source/base/vector.h"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class ReadableStateWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  ReadableStateWrapper() = default;

  ReadableStateWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = typename dish2::ReadableState< Spec >;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().peripheral.readable_state;
  }

  value_type* operator->() { return &operator*(); }

  ReadableStateWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  ReadableStateWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  ReadableStateWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  ReadableStateWrapper operator--(int) {
    ReadableStateWrapper res{ *this };
    res -= 1;
    return res;
  }

  ReadableStateWrapper operator+(const size_t rhs) {
    ReadableStateWrapper res{ *this };
    res += rhs;
    return res;
  }

  ReadableStateWrapper operator-(const size_t rhs) {
    ReadableStateWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_READABLESTATEWRAPPER_HPP_INCLUDE
