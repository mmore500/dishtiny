#pragma once
#ifndef DISH2_WORLD_ITERATORS_PERIPHERALCOUNTWRAPPER_HPP_INCLUDE
#define DISH2_WORLD_ITERATORS_PERIPHERALCOUNTWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../cell/Cell.hpp"
#include "../../genome/RootID.hpp"

namespace dish2 {

template<typename ParentIterator>
class PeripheralCountWrapper : public ParentIterator {

  using parent_t = ParentIterator;

  size_t lev;

public:

  PeripheralCountWrapper() = default;

  PeripheralCountWrapper(const parent_t& in, const size_t lev_)
  : parent_t(in), lev(lev_) {}

  using value_type = size_t;
  using pointer = value_type*;
  using reference = value_type&;

  value_type operator*() {
    return parent_t::operator*().IsPeripheral( lev );
  }

  value_type* operator->() { return &operator*(); }

  PeripheralCountWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  PeripheralCountWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  PeripheralCountWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  PeripheralCountWrapper operator--(int) {
    PeripheralCountWrapper res{ *this };
    res -= 1;
    return res;
  }

  PeripheralCountWrapper operator+(const size_t rhs) {
    PeripheralCountWrapper res{ *this };
    res += rhs;
    return res;
  }

  PeripheralCountWrapper operator-(const size_t rhs) {
    PeripheralCountWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_ITERATORS_PERIPHERALCOUNTWRAPPER_HPP_INCLUDE
