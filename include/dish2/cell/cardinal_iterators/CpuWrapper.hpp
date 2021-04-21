#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_CPUWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_CPUWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class CpuWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  // inherit constructors
  CpuWrapper() = default;

  CpuWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = typename Cardinal<Spec>::cpu_t;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() { return parent_t::operator*().cpu; }

  value_type* operator->() { return &operator*(); }

  CpuWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  CpuWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  CpuWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  CpuWrapper operator--(int) {
    return *this - 1;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  CpuWrapper operator+(const RHS rhs) {
    CpuWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  CpuWrapper operator-(const RHS rhs) {
    CpuWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_CPUWRAPPER_HPP_INCLUDE
