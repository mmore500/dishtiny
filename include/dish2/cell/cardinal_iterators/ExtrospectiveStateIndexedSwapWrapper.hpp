#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_EXTROSPECTIVESTATEINDEXEDSWAPWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_EXTROSPECTIVESTATEINDEXEDSWAPWRAPPER_HPP_INCLUDE

#include <limits>
#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

#include "_ReadableStateIndexedSwapper.hpp"

namespace dish2 {

template<typename Spec>
class ExtrospectiveStateIndexedSwapWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

  size_t idx{ std::numeric_limits<size_t>::max() };

public:

  ExtrospectiveStateIndexedSwapWrapper() = default;

  ExtrospectiveStateIndexedSwapWrapper(const parent_t& in) : parent_t(in) {}

  ExtrospectiveStateIndexedSwapWrapper(
    const parent_t& in, const size_t idx_
  ) : parent_t(in), idx( idx_) {}

  using value_type = dish2::ReadableStateIndexedSwapper<Spec>;
  using pointer = value_type*;
  using reference = value_type&;

  value_type operator*() {

    using target_type = typename dish2::ReadableState< Spec >;

    // we want to alter the cached last seen state in place
    auto& target = const_cast<target_type&>( std::as_const(
      parent_t::operator*().state_node_input
    ).Get() );

    return value_type{ target, idx };
  }

  value_type* operator->() { return &operator*(); }

  ExtrospectiveStateIndexedSwapWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  ExtrospectiveStateIndexedSwapWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  ExtrospectiveStateIndexedSwapWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  ExtrospectiveStateIndexedSwapWrapper operator--(int) {
    ExtrospectiveStateIndexedSwapWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  ExtrospectiveStateIndexedSwapWrapper operator+(const RHS rhs) {
    ExtrospectiveStateIndexedSwapWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  ExtrospectiveStateIndexedSwapWrapper operator-(const RHS rhs) {
    ExtrospectiveStateIndexedSwapWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_EXTROSPECTIVESTATEINDEXEDSWAPWRAPPER_HPP_INCLUDE
