#pragma once
#ifndef DISH2_WORLD_ITERATORS_ROOTIDVALWRAPPER_HPP_INCLUDE
#define DISH2_WORLD_ITERATORS_ROOTIDVALWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../cell/Cell.hpp"
#include "../../genome/RootID.hpp"

namespace dish2 {

template<typename ParentIterator>
class RootIDValWrapper : public ParentIterator {

  using parent_t = ParentIterator;

public:

  RootIDValWrapper() = default;

  RootIDValWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = size_t;
  using pointer = value_type*;
  using reference = value_type&;

  value_type operator*() {
    return parent_t::operator*().genome->root_id.GetID();
  }

  value_type* operator->() { return &operator*(); }

  RootIDValWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  RootIDValWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  RootIDValWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  RootIDValWrapper operator--(int) {
    RootIDValWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  RootIDValWrapper operator+(const RHS rhs) {
    RootIDValWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  RootIDValWrapper operator-(const RHS rhs) {
    RootIDValWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_ITERATORS_ROOTIDVALWRAPPER_HPP_INCLUDE
