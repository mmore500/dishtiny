#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_RESOURCENODEINPUTWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_RESOURCENODEINPUTWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/source/base/vector.h"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class ResourceNodeInputWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  ResourceNodeInputWrapper() = default;

  ResourceNodeInputWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = typename Cardinal<Spec>::resource_node_input_t;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().resource_node_input;
  }

  value_type* operator->() { return &operator*(); }

  ResourceNodeInputWrapper operator+(const size_t rhs) {
    ResourceNodeInputWrapper res{ *this };
    res += rhs;
    return res;
  }

  ResourceNodeInputWrapper operator-(const size_t rhs) {
    ResourceNodeInputWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_RESOURCENODEINPUTWRAPPER_HPP_INCLUDE
