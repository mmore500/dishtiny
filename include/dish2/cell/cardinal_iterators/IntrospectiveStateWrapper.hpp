#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_INTROSPECTIVESTATEWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_INTROSPECTIVESTATEWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class IntrospectiveStateWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  IntrospectiveStateWrapper() = default;

  IntrospectiveStateWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = typename dish2::IntrospectiveState< Spec >;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().peripheral.readable_state.template Get<
      value_type
    >();
  }

  value_type* operator->() { return &operator*(); }

  IntrospectiveStateWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  IntrospectiveStateWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  IntrospectiveStateWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  IntrospectiveStateWrapper operator--(int) {
    IntrospectiveStateWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  IntrospectiveStateWrapper operator+(const RHS rhs) {
    IntrospectiveStateWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  IntrospectiveStateWrapper operator-(const RHS rhs) {
    IntrospectiveStateWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_INTROSPECTIVESTATEWRAPPER_HPP_INCLUDE
