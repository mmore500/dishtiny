#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_ENTIREELAPSEDINSTRUCTIONCYCLESWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_ENTIREELAPSEDINSTRUCTIONCYCLESWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class EntireElapsedInstructionCyclesWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  // inherit constructors
  EntireElapsedInstructionCyclesWrapper() = default;

  EntireElapsedInstructionCyclesWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = size_t;
  using pointer = value_type*;
  using reference = value_type&;

  value_type operator*() const {
    return parent_t::operator*().cpu.GetCyclesSinceConstruction();
  }

  EntireElapsedInstructionCyclesWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  EntireElapsedInstructionCyclesWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  EntireElapsedInstructionCyclesWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  EntireElapsedInstructionCyclesWrapper operator--(int) {
    return *this - 1;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  EntireElapsedInstructionCyclesWrapper operator+(const RHS rhs) {
    EntireElapsedInstructionCyclesWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  EntireElapsedInstructionCyclesWrapper operator-(const RHS rhs) {
    EntireElapsedInstructionCyclesWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_ENTIREELAPSEDINSTRUCTIONCYCLESWRAPPER_HPP_INCLUDE
