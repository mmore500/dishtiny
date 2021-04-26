#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_RESOURCEINPUTPEEKWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_RESOURCEINPUTPEEKWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class ResourceInputPeekWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  ResourceInputPeekWrapper() = default;

  ResourceInputPeekWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = float;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().resource_node_input.Get();
  }

  value_type* operator->() { return &operator*(); }

  ResourceInputPeekWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  ResourceInputPeekWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  ResourceInputPeekWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  ResourceInputPeekWrapper operator--(int) {
    ResourceInputPeekWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  ResourceInputPeekWrapper operator+(const RHS rhs) {
    ResourceInputPeekWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  ResourceInputPeekWrapper operator-(const RHS rhs) {
    ResourceInputPeekWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_RESOURCEINPUTPEEKWRAPPER_HPP_INCLUDE
