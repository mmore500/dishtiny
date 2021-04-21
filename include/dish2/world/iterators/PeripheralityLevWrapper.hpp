#pragma once
#ifndef DISH2_WORLD_ITERATORS_PERIPHERALITYLEVWRAPPER_HPP_INCLUDE
#define DISH2_WORLD_ITERATORS_PERIPHERALITYLEVWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../cell/Cell.hpp"
#include "../../genome/RootID.hpp"

namespace dish2 {

template<typename ParentIterator>
class PeripheralityLevWrapper : public ParentIterator {

  using parent_t = ParentIterator;

  size_t lev;

public:

  PeripheralityLevWrapper() = default;

  PeripheralityLevWrapper(const parent_t& in, const size_t lev_)
  : parent_t(in), lev(lev_) {}

  using value_type = size_t;
  using pointer = value_type*;
  using reference = value_type&;

  value_type operator*() {
    return parent_t::operator*().GetPeripherality( lev );
  }

  value_type* operator->() { return &operator*(); }

  PeripheralityLevWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  PeripheralityLevWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  PeripheralityLevWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  PeripheralityLevWrapper operator--(int) {
    PeripheralityLevWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  PeripheralityLevWrapper operator+(const RHS rhs) {
    PeripheralityLevWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  PeripheralityLevWrapper operator-(const RHS rhs) {
    PeripheralityLevWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_ITERATORS_PERIPHERALITYLEVWRAPPER_HPP_INCLUDE
