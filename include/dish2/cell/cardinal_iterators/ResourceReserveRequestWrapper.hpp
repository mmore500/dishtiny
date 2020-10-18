#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_RESOURCERESERVEREQUESTWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_RESOURCERESERVEREQUESTWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/source/base/vector.h"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class ResourceReserveRequestWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  ResourceReserveRequestWrapper() = default;

  ResourceReserveRequestWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::ResourceReserveRequest;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().peripheral.readable_state.template Get<
      dish2::ResourceReserveRequest
    >();
  }

  value_type* operator->() { return &operator*(); }

  ResourceReserveRequestWrapper operator+(const size_t rhs) {
    ResourceReserveRequestWrapper res{ *this };
    res += rhs;
    return res;
  }

  ResourceReserveRequestWrapper operator-(const size_t rhs) {
    ResourceReserveRequestWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_RESOURCERESERVEREQUESTWRAPPER_HPP_INCLUDE
