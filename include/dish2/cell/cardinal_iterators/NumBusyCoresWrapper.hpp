#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_NUMBUSYCORESWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_NUMBUSYCORESWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class NumBusyCoresWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  // inherit constructors
  NumBusyCoresWrapper() = default;

  NumBusyCoresWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = size_t;
  using pointer = value_type*;
  using reference = value_type&;

  value_type operator*() const {
    return parent_t::operator*().cpu.GetNumBusyCores();
  }

  NumBusyCoresWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  NumBusyCoresWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  NumBusyCoresWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  NumBusyCoresWrapper operator--(int) {
    return *this - 1;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  NumBusyCoresWrapper operator+(const RHS rhs) {
    NumBusyCoresWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  NumBusyCoresWrapper operator-(const RHS rhs) {
    NumBusyCoresWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_NUMBUSYCORESWRAPPER_HPP_INCLUDE
