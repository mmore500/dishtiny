#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_GENOMENODEOUTPUTWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_GENOMENODEOUTPUTWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class GenomeNodeOutputWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  GenomeNodeOutputWrapper() = default;

  GenomeNodeOutputWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = typename Cardinal<Spec>::genome_node_output_t;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().genome_node_output;
  }

  value_type* operator->() { return &operator*(); }

  GenomeNodeOutputWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  GenomeNodeOutputWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  GenomeNodeOutputWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  GenomeNodeOutputWrapper operator--(int) {
    GenomeNodeOutputWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  GenomeNodeOutputWrapper operator+(const RHS rhs) {
    GenomeNodeOutputWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  GenomeNodeOutputWrapper operator-(const RHS rhs) {
    GenomeNodeOutputWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_GENOMENODEOUTPUTWRAPPER_HPP_INCLUDE
