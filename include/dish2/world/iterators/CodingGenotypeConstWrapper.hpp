#pragma once
#ifndef DISH2_WORLD_ITERATORS_CODINGGENOTYPECONSTWRAPPER_HPP_INCLUDE
#define DISH2_WORLD_ITERATORS_CODINGGENOTYPECONSTWRAPPER_HPP_INCLUDE

#include <type_traits>

#include "../../../../third-party/Empirical/include/emp/base/vector.hpp"

#include "../../cell/Cell.hpp"
#include "../../genome/RootID.hpp"

namespace dish2 {

template<typename Spec, typename ParentIterator>
class CodingGenotypeConstWrapper : public ParentIterator {

  using parent_t = ParentIterator;

public:

  CodingGenotypeConstWrapper() = default;

  CodingGenotypeConstWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = const std::tuple<
    const typename dish2::Genome<Spec>::event_tags_t&,
    const typename dish2::Genome<Spec>::program_t&
  >;
  using pointer = value_type*;
  using reference = value_type&;

  value_type operator*() {
    return value_type{
      parent_t::operator*().genome->event_tags,
      parent_t::operator*().genome->program
    };
  }

  CodingGenotypeConstWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  CodingGenotypeConstWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  CodingGenotypeConstWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  CodingGenotypeConstWrapper operator--(int) {
    CodingGenotypeConstWrapper res{ *this };
    res -= 1;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  CodingGenotypeConstWrapper operator+(const RHS rhs) {
    CodingGenotypeConstWrapper res{ *this };
    res += rhs;
    return res;
  }

  template<
    typename RHS,
    typename = typename std::enable_if<
      std::is_arithmetic<RHS>::value, RHS
    >::type
  >
  CodingGenotypeConstWrapper operator-(const RHS rhs) {
    CodingGenotypeConstWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_ITERATORS_CODINGGENOTYPECONSTWRAPPER_HPP_INCLUDE
