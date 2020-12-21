#pragma once
#ifndef DISH2_WORLD_ITERATORS_GENOTYPECONSTWRAPPER_HPP_INCLUDE
#define DISH2_WORLD_ITERATORS_GENOTYPECONSTWRAPPER_HPP_INCLUDE

#include "../../genome/Genome.hpp"

#include "LiveCellIterator.hpp"

namespace dish2 {

template<typename Spec>
class GenotypeConstWrapper : public dish2::LiveCellIterator<Spec> {

  using parent_t = dish2::LiveCellIterator<Spec>;

public:

  GenotypeConstWrapper() = default;

  GenotypeConstWrapper(const parent_t& in) : parent_t(in) {}

  using value_type = dish2::Genome<Spec>;
  using pointer = value_type*;
  using reference = value_type&;

  value_type operator*() { return *parent_t::operator*().genome; }

  GenotypeConstWrapper& operator++() {
    parent_t::operator++();
    return *this;
  }

  GenotypeConstWrapper operator++(int) {
    const auto res = *this;
    operator++();
    return res;
  }

  GenotypeConstWrapper& operator--() {
    parent_t::operator--();
    return *this;
  }

  GenotypeConstWrapper operator--(int) {
    GenotypeConstWrapper res{ *this };
    res -= 1;
    return res;
  }

  GenotypeConstWrapper operator+(const size_t rhs) {
    GenotypeConstWrapper res{ *this };
    res += rhs;
    return res;
  }

  GenotypeConstWrapper operator-(const size_t rhs) {
    GenotypeConstWrapper res{ *this };
    res -= rhs;
    return res;
  }

};

} // namespace dish2

#endif // #ifndef DISH2_WORLD_ITERATORS_GENOTYPECONSTWRAPPER_HPP_INCLUDE
