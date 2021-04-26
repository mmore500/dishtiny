#pragma once
#ifndef DISH2_WORLD_ITERATORS_ROOTIDCONSTWRAPPER_HPP_INCLUDE
#define DISH2_WORLD_ITERATORS_ROOTIDCONSTWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../cell/Cell.hpp"
#include "../../genome/RootID.hpp"

namespace dish2 {

template<typename ParentIterator>
class RootIDConstWrapper : public ParentIterator {

  using parent_t = ParentIterator;

public:

  RootIDConstWrapper() = default;

  RootIDConstWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = const dish2::RootID;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().genome->root_id;
  }

  value_type* operator->() { return &operator*(); }

  RootIDConstWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  RootIDConstWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  RootIDConstWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  RootIDConstWrapper operator--(int) {
    RootIDConstWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  RootIDConstWrapper operator+(const RHS rhs) {
    RootIDConstWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  RootIDConstWrapper operator-(const RHS rhs) {
    RootIDConstWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_ITERATORS_ROOTIDCONSTWRAPPER_HPP_INCLUDE
