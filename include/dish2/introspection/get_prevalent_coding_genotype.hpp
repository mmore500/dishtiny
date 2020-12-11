#pragma once
#ifndef DISH2_INTROSPECTION_GET_PREVALENT_CODING_GENOTYPE_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_PREVALENT_CODING_GENOTYPE_HPP_INCLUDE

#include <algorithm>
#include <map>
#include <utility>

#include "../../../third-party/Empirical/include/emp/datastructs/tuple_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "../cell/Cell.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/iterators/CodingGenotypeConstWrapper.hpp"
#include "../world/ThreadWorld.hpp"

namespace dish2 {

namespace internal::get_prevalent_coding_genotype {

template< typename Spec >
using wrapper_t = dish2::CodingGenotypeConstWrapper<
  Spec,
  dish2::LiveCellIterator<Spec>
>;

template< typename Spec >
using coding_genotype_ref_t = typename wrapper_t<Spec>::value_type;

template< typename Spec >
using ret_t = std::pair< coding_genotype_ref_t<Spec>, size_t >;

} // namespace internal::get_prevalent_coding_genotype

template< typename Spec >
internal::get_prevalent_coding_genotype::ret_t<Spec>
get_prevalent_coding_genotype( const dish2::ThreadWorld<Spec>& world ) {

  const auto& population = world.population;

  using wrapper_t = internal::get_prevalent_coding_genotype::wrapper_t<Spec>;

  // coding_genotype_ref_t -> count
  std::map< typename wrapper_t::value_type, size_t > counter;

  std::for_each(
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_begin( population ) },
    wrapper_t{ dish2::LiveCellIterator<Spec>::make_end( population ) },
    [&counter]( const auto& coding_genotype ){ ++counter[ coding_genotype ]; }
  );

  return *std::max_element(
    std::begin( counter ),
    std::end( counter ),
    []( const auto& left, const auto& right ) {
      const auto& [left_genotype, left_count] = left;
      const auto& [right_genotype, right_count] = right;
      return left_count < right_count;
    }
  );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_GET_PREVALENT_CODING_GENOTYPE_HPP_INCLUDE
