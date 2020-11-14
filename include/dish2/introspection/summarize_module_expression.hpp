#pragma once
#ifndef DISH2_INTROSPECTION_SUMMARIZE_MODULE_EXPRESSION_HPP_INCLUDE
#define DISH2_INTROSPECTION_SUMMARIZE_MODULE_EXPRESSION_HPP_INCLUDE

#include <algorithm>

#include "../../../third-party/header-only-pca/include/hopca/types.hpp"
#include "../../../third-party/signalgp-lite/include/sgpl/introspection/summarize_module_expression.hpp"

#include "../world/ThreadWorld.hpp"

#include "count_live_cardinals.hpp"
#include "get_maximum_module_count.hpp"

namespace dish2 {

template< typename Spec >
hopca::Matrix summarize_module_expression(
  const dish2::ThreadWorld< Spec >& world
) {

  const size_t max_modules = dish2::get_maximum_module_count<Spec>( world );
  const size_t num_live_cardinals = dish2::count_live_cardinals<Spec>( world );
  const auto& population = world.population;

  emp::vector<double> res;

  std::for_each(
    dish2::LiveCellIterator<Spec>::make_begin( population ),
    dish2::LiveCellIterator<Spec>::make_end( population ),
    [&]( const auto& cell ){ for (const auto& cardinal : cell) {

      auto cardinal_module_expression = sgpl::summarize_module_expression(
        cardinal.cpu,
        cell.genome->program
      );

      // pad with zeros if necessary
      cardinal_module_expression.resize( max_modules );

      std::copy(
        std::begin( cardinal_module_expression ),
        std::end( cardinal_module_expression ),
        std::back_inserter( res )
      );

    } }
  );

  return hola::matrix_from_array( res.data(), num_live_cardinals, max_modules );

}

} // namespace dish2

#endif // #ifndef DISH2_INTROSPECTION_SUMMARIZE_MODULE_EXPRESSION_HPP_INCLUDE
