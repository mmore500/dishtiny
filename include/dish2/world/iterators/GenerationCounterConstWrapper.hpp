#pragma once
#ifndef DISH2_WORLD_ITERATORS_GENERATIONCOUNTERCONSTWRAPPER_HPP_INCLUDE
#define DISH2_WORLD_ITERATORS_GENERATIONCOUNTERCONSTWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../cell/Cell.hpp"
#include "../../genome/RootID.hpp"

namespace dish2 {

template<typename Spec, typename ParentIterator>
class GenerationCounterConstWrapper : public ParentIterator {

  using parent_t = ParentIterator;

public:

  GenerationCounterConstWrapper() = default;

  GenerationCounterConstWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = const dish2::GenerationCounter< Spec >;
  using pointer = value_type*;
  using reference = value_type&;

  value_type& operator*() {
    return parent_t::operator*().genome->generation_counter;
  }

  value_type* operator->() { return &operator*(); }

  GenerationCounterConstWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  GenerationCounterConstWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  GenerationCounterConstWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  GenerationCounterConstWrapper operator--(int) {
    GenerationCounterConstWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  GenerationCounterConstWrapper operator+(const RHS rhs) {
    GenerationCounterConstWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  GenerationCounterConstWrapper operator-(const RHS rhs) {
    GenerationCounterConstWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_ITERATORS_GENERATIONCOUNTERCONSTWRAPPER_HPP_INCLUDE
