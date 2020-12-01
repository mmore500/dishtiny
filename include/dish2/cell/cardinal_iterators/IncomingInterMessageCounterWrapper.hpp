#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_INCOMINGINTERMESSAGECOUNTERWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_INCOMINGINTERMESSAGECOUNTERWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class IncomingInterMessageCounterWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  IncomingInterMessageCounterWrapper() = default;

  IncomingInterMessageCounterWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::IncomingInterMessageCounter;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().peripheral.readable_state.template Get<
      dish2::IncomingInterMessageCounter
    >();
  }

  value_type* operator->() { return &operator*(); }

  IncomingInterMessageCounterWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  IncomingInterMessageCounterWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  IncomingInterMessageCounterWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  IncomingInterMessageCounterWrapper operator--(int) {
    IncomingInterMessageCounterWrapper res{ *this };
    res -= 1;
    return res;
  }

  IncomingInterMessageCounterWrapper operator+(const size_t rhs) {
    IncomingInterMessageCounterWrapper res{ *this };
    res += rhs;
    return res;
  }

  IncomingInterMessageCounterWrapper operator-(const size_t rhs) {
    IncomingInterMessageCounterWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_INCOMINGINTERMESSAGECOUNTERWRAPPER_HPP_INCLUDE
