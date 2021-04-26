#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_PHYLOGENETICROOTMATCHWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_PHYLOGENETICROOTMATCHWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class PhylogeneticRootMatchWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  PhylogeneticRootMatchWrapper() = default;

  PhylogeneticRootMatchWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::PhylogeneticRootMatch;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().peripheral.readable_state.template Get<
      dish2::PhylogeneticRootMatch
    >();
  }

  value_type* operator->() { return &operator*(); }

  PhylogeneticRootMatchWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  PhylogeneticRootMatchWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  PhylogeneticRootMatchWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  PhylogeneticRootMatchWrapper operator--(int) {
    PhylogeneticRootMatchWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  PhylogeneticRootMatchWrapper operator+(const RHS rhs) {
    PhylogeneticRootMatchWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  PhylogeneticRootMatchWrapper operator-(const RHS rhs) {
    PhylogeneticRootMatchWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_PHYLOGENETICROOTMATCHWRAPPER_HPP_INCLUDE
