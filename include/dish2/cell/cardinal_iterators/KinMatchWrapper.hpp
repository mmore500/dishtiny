#pragma once
#ifndef DISH2_CELL_CARDINAL_ITERATORS_KINMATCHWRAPPER_HPP_INCLUDE
#define DISH2_CELL_CARDINAL_ITERATORS_KINMATCHWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/array.hpp"
#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

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

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  KinMatchWrapper operator+(const RHS rhs) {
    KinMatchWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  KinMatchWrapper operator-(const RHS rhs) {
    KinMatchWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_CELL_CARDINAL_ITERATORS_KINMATCHWRAPPER_HPP_INCLUDE
