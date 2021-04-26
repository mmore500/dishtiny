#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_WRITABLESTATEINDEXEDSWAPWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_WRITABLESTATEINDEXEDSWAPWRAPPER_HPP_INCLUDE

#include <limits>
#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/writable_state/WritableState.hpp"

#include "../Cardinal.hpp"

#include "_WritableStateIndexedSwapper.hpp"

namespace dish2 {

template<typename Spec>
class WritableStateIndexedSwapWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

  size_t idx{ std::numeric_limits<size_t>::max() };

public:

  WritableStateIndexedSwapWrapper() = default;

  WritableStateIndexedSwapWrapper(const parent_t& in) : parent_t(in) {}

  WritableStateIndexedSwapWrapper(
    const parent_t& in, const size_t idx_
  ) : parent_t(in), idx( idx_) {}

  using value_type = dish2::WritableStateIndexedSwapper<Spec>;
  using pointer = value_type*;
  using reference = value_type&;

  value_type operator*() {

    using target_type = typename dish2::WritableState< Spec >;

    auto& target = parent_t::operator*().peripheral.readable_state.template Get<
      target_type
    >();

    return value_type{ target, idx };
  }

  value_type* operator->() { return &operator*(); }

  WritableStateIndexedSwapWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  WritableStateIndexedSwapWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  WritableStateIndexedSwapWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  WritableStateIndexedSwapWrapper operator--(int) {
    WritableStateIndexedSwapWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  WritableStateIndexedSwapWrapper operator+(const RHS rhs) {
    WritableStateIndexedSwapWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  WritableStateIndexedSwapWrapper operator-(const RHS rhs) {
    WritableStateIndexedSwapWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_WRITABLESTATEINDEXEDSWAPWRAPPER_HPP_INCLUDE
