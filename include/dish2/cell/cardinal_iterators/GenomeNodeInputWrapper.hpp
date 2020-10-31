#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_GENOMENODEINPUTWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_GENOMENODEINPUTWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/source/base/vector.h"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class GenomeNodeInputWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  GenomeNodeInputWrapper() = default;

  GenomeNodeInputWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = typename Cardinal<Spec>::genome_node_input_t;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().genome_node_input;
  }

  value_type* operator->() { return &operator*(); }

  GenomeNodeInputWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  GenomeNodeInputWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  GenomeNodeInputWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  GenomeNodeInputWrapper operator--(int) {
    GenomeNodeInputWrapper res{ *this };
    res -= 1;
    return res;
  }

  GenomeNodeInputWrapper operator+(const size_t rhs) {
    GenomeNodeInputWrapper res{ *this };
    res += rhs;
    return res;
  }

  GenomeNodeInputWrapper operator-(const size_t rhs) {
    GenomeNodeInputWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_GENOMENODEINPUTWRAPPER_HPP_INCLUDE
