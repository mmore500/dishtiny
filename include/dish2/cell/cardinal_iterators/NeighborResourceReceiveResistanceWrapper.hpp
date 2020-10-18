#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_NEIGHBORRESOURCERECEIVERESISTANCEWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_NEIGHBORRESOURCERECEIVERESISTANCEWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/source/base/vector.h"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class NeighborResourceReceiveResistanceWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  NeighborResourceReceiveResistanceWrapper() = default;

  NeighborResourceReceiveResistanceWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::ResourceReceiveResistance;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().state_node_input.Get().template Get<
      dish2::ResourceReceiveResistance
    >();
  }

  value_type* operator->() { return &operator*(); }

  NeighborResourceReceiveResistanceWrapper operator+(const size_t rhs) {
    NeighborResourceReceiveResistanceWrapper res{ *this };
    res += rhs;
    return res;
  }

  NeighborResourceReceiveResistanceWrapper operator-(const size_t rhs) {
    NeighborResourceReceiveResistanceWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_NEIGHBORRESOURCERECEIVERESISTANCEWRAPPER_HPP_INCLUDE
