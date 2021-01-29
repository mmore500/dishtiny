#pragma once
#ifndef DISH2_INTROSPECTION_GET_PREVALENT_CODING_GENOTYPE_GENOME_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_PREVALENT_CODING_GENOTYPE_GENOME_HPP_INCLUDE

#include <algorithm>
#include <map>
#include <utility>

#include "../../../third-party/Empirical/include/emp/base/assert.hpp"

#include "get_prevalent_coding_genotype.hpp"
#include "no_live_cells.hpp"

namespace dish2 {

template< typename Spec >
std::pair< dish2::Genome<Spec>, size_t >
get_prevalent_coding_genotype_genome( const dish2::ThreadWorld<Spec>& world ) {

  const auto& [prevalent_coding_genotype, prevalence] =
    dish2::get_prevalent_coding_genotype( world );
  const auto& population = world.population;

  using wrapper_t = internal::get_prevalent_coding_genotype::wrapper_t<Spec>;

  for (
    auto it = dish2::LiveCellIterator<Spec>::make_begin( population );
    it != dish2::LiveCellIterator<Spec>::make_end( population );
    ++it
  ) if (
    *wrapper_t{ it } == prevalent_coding_genotype
  ) return std::pair{ *(it->genome), prevalence };

  emp_assert( dish2::no_live_cells<Spec>( world ) );
  return {};

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_PREVALENT_CODING_GENOTYPE_GENOME_HPP_INCLUDE
