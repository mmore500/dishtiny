#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_CONTROLLERMAPPEDSTATEWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_CONTROLLERMAPPEDSTATEWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class ControllerMappedStateWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  ControllerMappedStateWrapper() = default;

  ControllerMappedStateWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::ControllerMappedState< Spec >;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().peripheral.readable_state.template Get<
      dish2::ControllerMappedState< Spec >
    >();
  }

  value_type* operator->() { return &operator*(); }

  ControllerMappedStateWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  ControllerMappedStateWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  ControllerMappedStateWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  ControllerMappedStateWrapper operator--(int) {
    ControllerMappedStateWrapper res{ *this };
    res -= 1;
    return res;
  }

  ControllerMappedStateWrapper operator+(const size_t rhs) {
    ControllerMappedStateWrapper res{ *this };
    res += rhs;
    return res;
  }

  ControllerMappedStateWrapper operator-(const size_t rhs) {
    ControllerMappedStateWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_CONTROLLERMAPPEDSTATEWRAPPER_HPP_INCLUDE
