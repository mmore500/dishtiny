#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_KINGROUPIDVIEWWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_KINGROUPIDVIEWWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class KinGroupIDViewWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  KinGroupIDViewWrapper() = default;

  KinGroupIDViewWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::KinGroupIDView<Spec>;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().peripheral.readable_state.template Get<
      dish2::KinGroupIDView<Spec>
    >();
  }

  value_type* operator->() { return &operator*(); }

  KinGroupIDViewWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  KinGroupIDViewWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  KinGroupIDViewWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  KinGroupIDViewWrapper operator--(int) {
    KinGroupIDViewWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  KinGroupIDViewWrapper operator+(const RHS rhs) {
    KinGroupIDViewWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  KinGroupIDViewWrapper operator-(const RHS rhs) {
    KinGroupIDViewWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_KINGROUPIDVIEWWRAPPER_HPP_INCLUDE
