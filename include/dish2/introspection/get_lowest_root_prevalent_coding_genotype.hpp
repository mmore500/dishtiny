#pragma once
#ifndef DISH2_INTROSPECTION_GET_LOWEST_ROOT_PREVALENT_CODING_GENOTYPE_HPP_INCLUDE
#define DISH2_INTROSPECTION_GET_LOWEST_ROOT_PREVALENT_CODING_GENOTYPE_HPP_INCLUDE

#include <algorithm>
#include <limits>
#include <map>
#include <utility>

#include "../../../third-party/conduit/include/uitsl/algorithm/for_each.hpp"
#include "../../../third-party/Empirical/include/emp/base/assert.hpp"
#include "../../../third-party/Empirical/include/emp/datastructs/tuple_utils.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/count_modules.hpp"

#include "../cell/Cell.hpp"
#include "../genome/Genome.hpp"
#include "../world/iterators/CodingGenotypeConstWrapper.hpp"
#include "../world/iterators/LiveCellIterator.hpp"
#include "../world/iterators/RootIDValWrapper.hpp"
#include "../world/ThreadWorld.hpp"

#include "get_lowest_root_id.hpp"
#include "get_prevalent_coding_genotype.hpp"
#include "no_live_cells.hpp"

namespace dish2 {

template< typename Spec >
internal::get_prevalent_coding_genotype::ret_t<Spec>
get_lowest_root_prevalent_coding_genotype(
  const dish2::ThreadWorld<Spec>& world
) {

  const size_t lowest_root_id = dish2::get_lowest_root_id<Spec>( world );

  const auto& population = world.population;

  using wrapper_t = internal::get_prevalent_coding_genotype::wrapper_t<Spec>;
  using coding_genotype_ref_t = typename wrapper_t::value_type;

  const auto begin = dish2::LiveCellIterator<Spec>::make_begin( population );
  const auto end = dish2::LiveCellIterator<Spec>::make_end( population );

  // coding_genotype_ref_t -> count
  std::map< coding_genotype_ref_t, size_t > counter;

  uitsl::for_each(
    wrapper_t{ begin }, wrapper_t{ end },
    dish2::RootIDValWrapper<decltype(begin)>{ begin },
    [&counter, lowest_root_id](
      const auto& coding_genotype, const size_t root_id
    ){
      if (root_id == lowest_root_id) ++counter[ coding_genotype ];
    }
  );

  const static dish2::Genome<Spec> blank_dummy{};
  if ( counter.empty() ) {
    emp_assert( dish2::no_live_cells<Spec>( world ) );
    return std::pair{
      coding_genotype_ref_t{ blank_dummy.event_tags, blank_dummy.program },
      0
    };
  } else return *std::max_element(
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

#endif // #ifndef DISH2_INTROSPECTION_GET_LOWEST_ROOT_PREVALENT_CODING_GENOTYPE_HPP_INCLUDE
