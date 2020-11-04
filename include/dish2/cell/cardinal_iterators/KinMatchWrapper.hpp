#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_KINMATCHWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_KINMATCHWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/source/base/array.h"
#include "../../../../third-party/Empirical/source/base/vector.h"

#include "../../peripheral/readable_state/ReadableState.hpp"

#include "../Cardinal.hpp"

namespace dish2 {

template<typename Spec>
class KinMatchWrapper
: public emp::vector<Cardinal<Spec>>::iterator {

  using parent_t = typename emp::vector<Cardinal<Spec>>::iterator;

public:

  KinMatchWrapper() = default;

  KinMatchWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = emp::array<char, Spec::NLEV>;
  using pointer = value_type*;
  using reference = value_type&;

  value_type operator*() {
    value_type res;
    for (size_t lev{}; lev < Spec::NLEV; ++lev) {
      res[lev] = parent_t::operator*().IsNeighborKin( lev );
    }
    return res;
  }

  KinMatchWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  KinMatchWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  KinMatchWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  KinMatchWrapper operator--(int) {
    KinMatchWrapper res{ *this };
    res -= 1;
    return res;
  }

  KinMatchWrapper operator+(const size_t rhs) {
    KinMatchWrapper res{ *this };
    res += rhs;
    return res;
  }

  KinMatchWrapper operator-(const size_t rhs) {
    KinMatchWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_KINMATCHWRAPPER_HPP_INCLUDE
