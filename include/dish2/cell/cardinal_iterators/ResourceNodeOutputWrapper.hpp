#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_RESOURCENODEOUTPUTWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_RESOURCENODEOUTPUTWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class ResourceNodeOutputWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  ResourceNodeOutputWrapper() = default;

  ResourceNodeOutputWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = typename Cardinal<Spec>::resource_node_output_t;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().resource_node_output;
  }

  value_type* operator->() { return &operator*(); }

  ResourceNodeOutputWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  ResourceNodeOutputWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  ResourceNodeOutputWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  ResourceNodeOutputWrapper operator--(int) {
    ResourceNodeOutputWrapper res{ *this };
    res -= 1;
    return res;
  }

  ResourceNodeOutputWrapper operator+(const size_t rhs) {
    ResourceNodeOutputWrapper res{ *this };
    res += rhs;
    return res;
  }

  ResourceNodeOutputWrapper operator-(const size_t rhs) {
    ResourceNodeOutputWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_RESOURCENODEOUTPUTWRAPPER_HPP_INCLUDE
