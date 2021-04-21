#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_EXTROSPECTIVESTATEWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_EXTROSPECTIVESTATEWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class ExtrospectiveStateWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  ExtrospectiveStateWrapper() = default;

  ExtrospectiveStateWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = typename dish2::ReadableState< Spec >;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    // we want to alter the cached last seen state in place
    return const_cast<value_type&>( std::as_const(
      parent_t::operator*().state_node_input
    ).Get() );
  }

  value_type* operator->() { return &operator*(); }

  ExtrospectiveStateWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  ExtrospectiveStateWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  ExtrospectiveStateWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  ExtrospectiveStateWrapper operator--(int) {
    ExtrospectiveStateWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  ExtrospectiveStateWrapper operator+(const RHS rhs) {
    ExtrospectiveStateWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  ExtrospectiveStateWrapper operator-(const RHS rhs) {
    ExtrospectiveStateWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_EXTROSPECTIVESTATEWRAPPER_HPP_INCLUDE
