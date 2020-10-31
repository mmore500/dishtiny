#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_APOPTOSISREQUESTWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_APOPTOSISREQUESTWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/source/base/vector.h"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class ApoptosisRequestWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  ApoptosisRequestWrapper() = default;

  ApoptosisRequestWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::ApoptosisRequest;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().peripheral.readable_state.template Get<
      dish2::ApoptosisRequest
    >();
  }

  value_type* operator->() { return &operator*(); }

  ApoptosisRequestWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  ApoptosisRequestWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  ApoptosisRequestWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  ApoptosisRequestWrapper operator--(int) {
    ApoptosisRequestWrapper res{ *this };
    res -= 1;
    return res;
  }

  ApoptosisRequestWrapper operator+(const size_t rhs) {
    ApoptosisRequestWrapper res{ *this };
    res += rhs;
    return res;
  }

  ApoptosisRequestWrapper operator-(const size_t rhs) {
    ApoptosisRequestWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_APOPTOSISREQUESTWRAPPER_HPP_INCLUDE
