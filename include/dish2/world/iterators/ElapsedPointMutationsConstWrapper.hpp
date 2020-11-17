#pragma once
#ifndef DISH2_WORLD_ITERATORS_ELAPSEDPOINTMUTATIONSCONSTWRAPPER_HPP_INCLUDE
#define DISH2_WORLD_ITERATORS_ELAPSEDPOINTMUTATIONSCONSTWRAPPER_HPP_INCLUDE

#include "../../../../third-party/Empirical/source/base/vector.h"

#include "../../cell/Cell.hpp"
#include "../../genome/RootID.hpp"

namespace dish2 {

template<typename Spec, typename ParentIterator>
class ElapsedPointMutationsConstWrapper : public ParentIterator {

  using parent_t = ParentIterator;

public:

  ElapsedPointMutationsConstWrapper() = default;

  ElapsedPointMutationsConstWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = const size_t;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*(
      ).genome->mutation_counter.point_mutation_counter;
  }

  ElapsedPointMutationsConstWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  ElapsedPointMutationsConstWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  ElapsedPointMutationsConstWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  ElapsedPointMutationsConstWrapper operator--(int) {
    ElapsedPointMutationsConstWrapper res{ *this };
    res -= 1;
    return res;
  }

  ElapsedPointMutationsConstWrapper operator+(const size_t rhs) {
    ElapsedPointMutationsConstWrapper res{ *this };
    res += rhs;
    return res;
  }

  ElapsedPointMutationsConstWrapper operator-(const size_t rhs) {
    ElapsedPointMutationsConstWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_ITERATORS_ELAPSEDPOINTMUTATIONSCONSTWRAPPER_HPP_INCLUDE
