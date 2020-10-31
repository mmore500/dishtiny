#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_INTRAMESSAGENODEWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_INTRAMESSAGENODEWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/source/base/vector.h"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class IntraMessageNodeWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  IntraMessageNodeWrapper() = default;

  IntraMessageNodeWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = typename Cardinal<Spec>::intra_message_node_t;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() { return parent_t::operator*().intra_message_node; }

  value_type* operator->() { return &operator*(); }

  IntraMessageNodeWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  IntraMessageNodeWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  IntraMessageNodeWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  IntraMessageNodeWrapper operator--(int) {
    IntraMessageNodeWrapper res{ *this };
    res -= 1;
    return res;
  }

  IntraMessageNodeWrapper operator+(const size_t rhs) {
    IntraMessageNodeWrapper res{ *this };
    res += rhs;
    return res;
  }

  IntraMessageNodeWrapper operator-(const size_t rhs) {
    IntraMessageNodeWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_INTRAMESSAGENODEWRAPPER_HPP_INCLUDE
